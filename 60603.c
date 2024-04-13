static void start_multicast(void)
{
    FFServerStream *stream;
    char session_id[32];
    HTTPContext *rtp_c;
    struct sockaddr_in dest_addr = {0};
    int default_port, stream_index;
    unsigned int random0, random1;

    default_port = 6000;
    for(stream = config.first_stream; stream; stream = stream->next) {

        if (!stream->is_multicast)
            continue;

        random0 = av_lfg_get(&random_state);
        random1 = av_lfg_get(&random_state);

        /* open the RTP connection */
        snprintf(session_id, sizeof(session_id), "%08x%08x", random0, random1);

        /* choose a port if none given */
        if (stream->multicast_port == 0) {
            stream->multicast_port = default_port;
            default_port += 100;
        }

        dest_addr.sin_family = AF_INET;
        dest_addr.sin_addr = stream->multicast_ip;
        dest_addr.sin_port = htons(stream->multicast_port);

        rtp_c = rtp_new_connection(&dest_addr, stream, session_id,
                                   RTSP_LOWER_TRANSPORT_UDP_MULTICAST);
        if (!rtp_c)
            continue;

        if (open_input_stream(rtp_c, "") < 0) {
            http_log("Could not open input stream for stream '%s'\n",
                     stream->filename);
            continue;
        }

        /* open each RTP stream */
        for(stream_index = 0; stream_index < stream->nb_streams;
            stream_index++) {
            dest_addr.sin_port = htons(stream->multicast_port +
                                       2 * stream_index);
            if (rtp_new_av_stream(rtp_c, stream_index, &dest_addr, NULL) >= 0)
                continue;

            http_log("Could not open output stream '%s/streamid=%d'\n",
                     stream->filename, stream_index);
            exit(1);
        }

        rtp_c->state = HTTPSTATE_SEND_DATA;
    }
}
