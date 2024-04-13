static void close_connection(HTTPContext *c)
{
    HTTPContext **cp, *c1;
    int i, nb_streams;
    AVFormatContext *ctx;
    AVStream *st;

    /* remove connection from list */
    cp = &first_http_ctx;
    while (*cp) {
        c1 = *cp;
        if (c1 == c)
            *cp = c->next;
        else
            cp = &c1->next;
    }

    /* remove references, if any (XXX: do it faster) */
    for(c1 = first_http_ctx; c1; c1 = c1->next) {
        if (c1->rtsp_c == c)
            c1->rtsp_c = NULL;
    }

    /* remove connection associated resources */
    if (c->fd >= 0)
        closesocket(c->fd);
    if (c->fmt_in) {
        /* close each frame parser */
        for(i=0;i<c->fmt_in->nb_streams;i++) {
            st = c->fmt_in->streams[i];
            if (st->codec->codec)
                avcodec_close(st->codec);
        }
        avformat_close_input(&c->fmt_in);
    }

    /* free RTP output streams if any */
    nb_streams = 0;
    if (c->stream)
        nb_streams = c->stream->nb_streams;

    for(i=0;i<nb_streams;i++) {
        ctx = c->rtp_ctx[i];
        if (ctx) {
            av_write_trailer(ctx);
            av_dict_free(&ctx->metadata);
            av_freep(&ctx->streams[0]);
            av_freep(&ctx);
        }
        ffurl_close(c->rtp_handles[i]);
    }

    ctx = c->pfmt_ctx;

    if (ctx) {
        if (!c->last_packet_sent && c->state == HTTPSTATE_SEND_DATA_TRAILER) {
            /* prepare header */
            if (ctx->oformat && avio_open_dyn_buf(&ctx->pb) >= 0) {
                av_write_trailer(ctx);
                av_freep(&c->pb_buffer);
                avio_close_dyn_buf(ctx->pb, &c->pb_buffer);
            }
        }
        for(i=0; i<ctx->nb_streams; i++)
            av_freep(&ctx->streams[i]);
        av_freep(&ctx->streams);
        av_freep(&ctx->priv_data);
        }

    if (c->stream && !c->post && c->stream->stream_type == STREAM_TYPE_LIVE)
        current_bandwidth -= c->stream->bandwidth;

    /* signal that there is no feed if we are the feeder socket */
    if (c->state == HTTPSTATE_RECEIVE_DATA && c->stream) {
        c->stream->feed_opened = 0;
        close(c->feed_fd);
    }

    av_freep(&c->pb_buffer);
    av_freep(&c->packet_buffer);
    av_freep(&c->buffer);
    av_free(c);
    nb_connections--;
}
