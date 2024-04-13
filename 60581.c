static int http_server(void)
{
    int server_fd = 0, rtsp_server_fd = 0;
    int ret, delay;
    struct pollfd *poll_table, *poll_entry;
    HTTPContext *c, *c_next;

    poll_table = av_mallocz_array(config.nb_max_http_connections + 2,
                                  sizeof(*poll_table));
    if(!poll_table) {
        http_log("Impossible to allocate a poll table handling %d "
                 "connections.\n", config.nb_max_http_connections);
        return -1;
    }

    if (config.http_addr.sin_port) {
        server_fd = socket_open_listen(&config.http_addr);
        if (server_fd < 0)
            goto quit;
    }

    if (config.rtsp_addr.sin_port) {
        rtsp_server_fd = socket_open_listen(&config.rtsp_addr);
        if (rtsp_server_fd < 0) {
            closesocket(server_fd);
            goto quit;
        }
    }

    if (!rtsp_server_fd && !server_fd) {
        http_log("HTTP and RTSP disabled.\n");
        goto quit;
    }

    http_log("FFserver started.\n");

    start_children(config.first_feed);

    start_multicast();

    for(;;) {
        poll_entry = poll_table;
        if (server_fd) {
            poll_entry->fd = server_fd;
            poll_entry->events = POLLIN;
            poll_entry++;
        }
        if (rtsp_server_fd) {
            poll_entry->fd = rtsp_server_fd;
            poll_entry->events = POLLIN;
            poll_entry++;
        }

        /* wait for events on each HTTP handle */
        c = first_http_ctx;
        delay = 1000;
        while (c) {
            int fd;
            fd = c->fd;
            switch(c->state) {
            case HTTPSTATE_SEND_HEADER:
            case RTSPSTATE_SEND_REPLY:
            case RTSPSTATE_SEND_PACKET:
                c->poll_entry = poll_entry;
                poll_entry->fd = fd;
                poll_entry->events = POLLOUT;
                poll_entry++;
                break;
            case HTTPSTATE_SEND_DATA_HEADER:
            case HTTPSTATE_SEND_DATA:
            case HTTPSTATE_SEND_DATA_TRAILER:
                if (!c->is_packetized) {
                    /* for TCP, we output as much as we can
                     * (may need to put a limit) */
                    c->poll_entry = poll_entry;
                    poll_entry->fd = fd;
                    poll_entry->events = POLLOUT;
                    poll_entry++;
                } else {
                    /* when ffserver is doing the timing, we work by
                     * looking at which packet needs to be sent every
                     * 10 ms (one tick wait XXX: 10 ms assumed) */
                    if (delay > 10)
                        delay = 10;
                }
                break;
            case HTTPSTATE_WAIT_REQUEST:
            case HTTPSTATE_RECEIVE_DATA:
            case HTTPSTATE_WAIT_FEED:
            case RTSPSTATE_WAIT_REQUEST:
                /* need to catch errors */
                c->poll_entry = poll_entry;
                poll_entry->fd = fd;
                poll_entry->events = POLLIN;/* Maybe this will work */
                poll_entry++;
                break;
            default:
                c->poll_entry = NULL;
                break;
            }
            c = c->next;
        }

        /* wait for an event on one connection. We poll at least every
         * second to handle timeouts */
        do {
            ret = poll(poll_table, poll_entry - poll_table, delay);
            if (ret < 0 && ff_neterrno() != AVERROR(EAGAIN) &&
                ff_neterrno() != AVERROR(EINTR)) {
                goto quit;
            }
        } while (ret < 0);

        cur_time = av_gettime() / 1000;

        if (need_to_start_children) {
            need_to_start_children = 0;
            start_children(config.first_feed);
        }

        /* now handle the events */
        for(c = first_http_ctx; c; c = c_next) {
            c_next = c->next;
            if (handle_connection(c) < 0) {
                log_connection(c);
                /* close and free the connection */
                close_connection(c);
            }
        }

        poll_entry = poll_table;
        if (server_fd) {
            /* new HTTP connection request ? */
            if (poll_entry->revents & POLLIN)
                new_connection(server_fd, 0);
            poll_entry++;
        }
        if (rtsp_server_fd) {
            /* new RTSP connection request ? */
            if (poll_entry->revents & POLLIN)
                new_connection(rtsp_server_fd, 1);
        }
    }

quit:
    av_free(poll_table);
    return -1;
}
