process_client_headers (struct conn_s *connptr, orderedmap hashofheaders)
{
        static const char *skipheaders[] = {
                "host",
                "keep-alive",
                "proxy-connection",
                "te",
                "trailers",
                "upgrade"
        };
        int i;
        size_t iter;
        int ret = 0;

        char *data, *header;

        /*
         * Don't send headers if there's already an error, if the request was
         * a stats request, or if this was a CONNECT method (unless upstream
         * http proxy is in use.)
         */
        if (connptr->server_fd == -1 || connptr->show_stats
            || (connptr->connect_method && ! UPSTREAM_IS_HTTP(connptr))) {
                log_message (LOG_INFO,
                             "Not sending client headers to remote machine");
                return 0;
        }

        /*
         * See if there is a "Content-Length" header.  If so, again we need
         * to do a bit of processing.
         */
        connptr->content_length.client = get_content_length (hashofheaders);

        /* Check whether client sends chunked data. */
        if (connptr->content_length.client == -1 && is_chunked_transfer (hashofheaders))
                connptr->content_length.client = -2;

        /*
         * See if there is a "Connection" header.  If so, we need to do a bit
         * of processing. :)
         */
        remove_connection_headers (hashofheaders);

        /*
         * Delete the headers listed in the skipheaders list
         */
        for (i = 0; i != (sizeof (skipheaders) / sizeof (char *)); i++) {
                orderedmap_remove (hashofheaders, skipheaders[i]);
        }

        /* Send, or add the Via header */
        ret = write_via_header (connptr->server_fd, hashofheaders,
                                connptr->protocol.major,
                                connptr->protocol.minor);
        if (ret < 0) {
                indicate_http_error (connptr, 503,
                                     "Could not send data to remote server",
                                     "detail",
                                     "A network error occurred while "
                                     "trying to write data to the remote web server.",
                                     NULL);
                goto PULL_CLIENT_DATA;
        }

        /*
         * Output all the remaining headers to the remote machine.
         */
        iter = 0;
        while((iter = orderedmap_next(hashofheaders, iter, &data, &header))) {
                if (!is_anonymous_enabled (config)
                    || anonymous_search (config, data) > 0) {
                        ret =
                            write_message (connptr->server_fd,
                                           "%s: %s\r\n", data, header);
                        if (ret < 0) {
                                indicate_http_error (connptr, 503,
                                                     "Could not send data to remote server",
                                                     "detail",
                                                     "A network error occurred while "
                                                     "trying to write data to the "
                                                     "remote web server.",
                                                     NULL);
                                goto PULL_CLIENT_DATA;
                        }
                }
        }
#if defined(XTINYPROXY_ENABLE)
        if (config->add_xtinyproxy)
                add_xtinyproxy_header (connptr);
#endif

        /* Write the final "blank" line to signify the end of the headers */
        if (safe_write (connptr->server_fd, "\r\n", 2) < 0)
                return -1;

        /*
         * Spin here pulling the data from the client.
         */
PULL_CLIENT_DATA:
        if (connptr->content_length.client > 0) {
                ret = pull_client_data (connptr,
                                        connptr->content_length.client, 1);
        } else if (connptr->content_length.client == -2)
                ret = pull_client_data_chunked (connptr);

        return ret;
}