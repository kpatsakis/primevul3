static HTTPContext *rtp_new_connection(struct sockaddr_in *from_addr,
                                       FFServerStream *stream,
                                       const char *session_id,
                                       enum RTSPLowerTransport rtp_protocol)
{
    HTTPContext *c = NULL;
    const char *proto_str;

    /* XXX: should output a warning page when coming
     * close to the connection limit */
    if (nb_connections >= config.nb_max_connections)
        goto fail;

    /* add a new connection */
    c = av_mallocz(sizeof(HTTPContext));
    if (!c)
        goto fail;

    c->fd = -1;
    c->poll_entry = NULL;
    c->from_addr = *from_addr;
    c->buffer_size = IOBUFFER_INIT_SIZE;
    c->buffer = av_malloc(c->buffer_size);
    if (!c->buffer)
        goto fail;
    nb_connections++;
    c->stream = stream;
    av_strlcpy(c->session_id, session_id, sizeof(c->session_id));
    c->state = HTTPSTATE_READY;
    c->is_packetized = 1;
    c->rtp_protocol = rtp_protocol;

    /* protocol is shown in statistics */
    switch(c->rtp_protocol) {
    case RTSP_LOWER_TRANSPORT_UDP_MULTICAST:
        proto_str = "MCAST";
        break;
    case RTSP_LOWER_TRANSPORT_UDP:
        proto_str = "UDP";
        break;
    case RTSP_LOWER_TRANSPORT_TCP:
        proto_str = "TCP";
        break;
    default:
        proto_str = "???";
        break;
    }
    av_strlcpy(c->protocol, "RTP/", sizeof(c->protocol));
    av_strlcat(c->protocol, proto_str, sizeof(c->protocol));

    current_bandwidth += stream->bandwidth;

    c->next = first_http_ctx;
    first_http_ctx = c;
    return c;

 fail:
    if (c) {
        av_freep(&c->buffer);
        av_free(c);
    }
    return NULL;
}
