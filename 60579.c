static int http_prepare_data(HTTPContext *c)
{
    int i, len, ret;
    AVFormatContext *ctx;

    av_freep(&c->pb_buffer);
    switch(c->state) {
    case HTTPSTATE_SEND_DATA_HEADER:
        ctx = avformat_alloc_context();
        if (!ctx)
            return AVERROR(ENOMEM);
        c->pfmt_ctx = ctx;
        av_dict_copy(&(c->pfmt_ctx->metadata), c->stream->metadata, 0);

        for(i=0;i<c->stream->nb_streams;i++) {
            LayeredAVStream *src;
            AVStream *st = avformat_new_stream(c->pfmt_ctx, NULL);
            if (!st)
                return AVERROR(ENOMEM);

            /* if file or feed, then just take streams from FFServerStream
             * struct */
            if (!c->stream->feed ||
                c->stream->feed == c->stream)
                src = c->stream->streams[i];
            else
                src = c->stream->feed->streams[c->stream->feed_streams[i]];

            unlayer_stream(c->pfmt_ctx->streams[i], src); //TODO we no longer copy st->internal, does this matter?
            av_assert0(!c->pfmt_ctx->streams[i]->priv_data);

            if (src->codec->flags & AV_CODEC_FLAG_BITEXACT)
                c->pfmt_ctx->flags |= AVFMT_FLAG_BITEXACT;
        }
        /* set output format parameters */
        c->pfmt_ctx->oformat = c->stream->fmt;
        av_assert0(c->pfmt_ctx->nb_streams == c->stream->nb_streams);

        c->got_key_frame = 0;

        /* prepare header and save header data in a stream */
        if (avio_open_dyn_buf(&c->pfmt_ctx->pb) < 0) {
            /* XXX: potential leak */
            return -1;
        }
        c->pfmt_ctx->pb->seekable = 0;

        /*
         * HACK to avoid MPEG-PS muxer to spit many underflow errors
         * Default value from FFmpeg
         * Try to set it using configuration option
         */
        c->pfmt_ctx->max_delay = (int)(0.7*AV_TIME_BASE);

        if ((ret = avformat_write_header(c->pfmt_ctx, NULL)) < 0) {
            http_log("Error writing output header for stream '%s': %s\n",
                     c->stream->filename, av_err2str(ret));
            return ret;
        }
        av_dict_free(&c->pfmt_ctx->metadata);

        len = avio_close_dyn_buf(c->pfmt_ctx->pb, &c->pb_buffer);
        c->buffer_ptr = c->pb_buffer;
        c->buffer_end = c->pb_buffer + len;

        c->state = HTTPSTATE_SEND_DATA;
        c->last_packet_sent = 0;
        break;
    case HTTPSTATE_SEND_DATA:
        /* find a new packet */
        /* read a packet from the input stream */
        if (c->stream->feed)
            ffm_set_write_index(c->fmt_in,
                                c->stream->feed->feed_write_index,
                                c->stream->feed->feed_size);

        if (c->stream->max_time &&
            c->stream->max_time + c->start_time - cur_time < 0)
            /* We have timed out */
            c->state = HTTPSTATE_SEND_DATA_TRAILER;
        else {
            AVPacket pkt;
        redo:
            ret = av_read_frame(c->fmt_in, &pkt);
            if (ret < 0) {
                if (c->stream->feed) {
                    /* if coming from feed, it means we reached the end of the
                     * ffm file, so must wait for more data */
                    c->state = HTTPSTATE_WAIT_FEED;
                    return 1; /* state changed */
                }
                if (ret == AVERROR(EAGAIN)) {
                    /* input not ready, come back later */
                    return 0;
                }
                if (c->stream->loop) {
                    avformat_close_input(&c->fmt_in);
                    if (open_input_stream(c, "") < 0)
                        goto no_loop;
                    goto redo;
                } else {
                    no_loop:
                        /* must send trailer now because EOF or error */
                        c->state = HTTPSTATE_SEND_DATA_TRAILER;
                }
            } else {
                int source_index = pkt.stream_index;
                /* update first pts if needed */
                if (c->first_pts == AV_NOPTS_VALUE && pkt.dts != AV_NOPTS_VALUE) {
                    c->first_pts = av_rescale_q(pkt.dts, c->fmt_in->streams[pkt.stream_index]->time_base, AV_TIME_BASE_Q);
                    c->start_time = cur_time;
                }
                /* send it to the appropriate stream */
                if (c->stream->feed) {
                    /* if coming from a feed, select the right stream */
                    if (c->switch_pending) {
                        c->switch_pending = 0;
                        for(i=0;i<c->stream->nb_streams;i++) {
                            if (c->switch_feed_streams[i] == pkt.stream_index)
                                if (pkt.flags & AV_PKT_FLAG_KEY)
                                    c->switch_feed_streams[i] = -1;
                            if (c->switch_feed_streams[i] >= 0)
                                c->switch_pending = 1;
                        }
                    }
                    for(i=0;i<c->stream->nb_streams;i++) {
                        if (c->stream->feed_streams[i] == pkt.stream_index) {
                            AVStream *st = c->fmt_in->streams[source_index];
                            pkt.stream_index = i;
                            if (pkt.flags & AV_PKT_FLAG_KEY &&
                                (st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ||
                                 c->stream->nb_streams == 1))
                                c->got_key_frame = 1;
                            if (!c->stream->send_on_key || c->got_key_frame)
                                goto send_it;
                        }
                    }
                } else {
                    AVStream *ist, *ost;
                send_it:
                    ist = c->fmt_in->streams[source_index];
                    /* specific handling for RTP: we use several
                     * output streams (one for each RTP connection).
                     * XXX: need more abstract handling */
                    if (c->is_packetized) {
                        /* compute send time and duration */
                        if (pkt.dts != AV_NOPTS_VALUE) {
                            c->cur_pts = av_rescale_q(pkt.dts, ist->time_base, AV_TIME_BASE_Q);
                            c->cur_pts -= c->first_pts;
                        }
                        c->cur_frame_duration = av_rescale_q(pkt.duration, ist->time_base, AV_TIME_BASE_Q);
                        /* find RTP context */
                        c->packet_stream_index = pkt.stream_index;
                        ctx = c->rtp_ctx[c->packet_stream_index];
                        if(!ctx) {
                            av_packet_unref(&pkt);
                            break;
                        }
                        /* only one stream per RTP connection */
                        pkt.stream_index = 0;
                    } else {
                        ctx = c->pfmt_ctx;
                        /* Fudge here */
                    }

                    if (c->is_packetized) {
                        int max_packet_size;
                        if (c->rtp_protocol == RTSP_LOWER_TRANSPORT_TCP)
                            max_packet_size = RTSP_TCP_MAX_PACKET_SIZE;
                        else
                            max_packet_size = c->rtp_handles[c->packet_stream_index]->max_packet_size;
                        ret = ffio_open_dyn_packet_buf(&ctx->pb,
                                                       max_packet_size);
                    } else
                        ret = avio_open_dyn_buf(&ctx->pb);

                    if (ret < 0) {
                        /* XXX: potential leak */
                        return -1;
                    }
                    ost = ctx->streams[pkt.stream_index];

                    ctx->pb->seekable = 0;
                    if (pkt.dts != AV_NOPTS_VALUE)
                        pkt.dts = av_rescale_q(pkt.dts, ist->time_base,
                                               ost->time_base);
                    if (pkt.pts != AV_NOPTS_VALUE)
                        pkt.pts = av_rescale_q(pkt.pts, ist->time_base,
                                               ost->time_base);
                    pkt.duration = av_rescale_q(pkt.duration, ist->time_base,
                                                ost->time_base);
                    if ((ret = av_write_frame(ctx, &pkt)) < 0) {
                        http_log("Error writing frame to output for stream '%s': %s\n",
                                 c->stream->filename, av_err2str(ret));
                        c->state = HTTPSTATE_SEND_DATA_TRAILER;
                    }

                    av_freep(&c->pb_buffer);
                    len = avio_close_dyn_buf(ctx->pb, &c->pb_buffer);
                    ctx->pb = NULL;
                    c->cur_frame_bytes = len;
                    c->buffer_ptr = c->pb_buffer;
                    c->buffer_end = c->pb_buffer + len;

                    if (len == 0) {
                        av_packet_unref(&pkt);
                        goto redo;
                    }
                }
                av_packet_unref(&pkt);
            }
        }
        break;
    default:
    case HTTPSTATE_SEND_DATA_TRAILER:
        /* last packet test ? */
        if (c->last_packet_sent || c->is_packetized)
            return -1;
        ctx = c->pfmt_ctx;
        /* prepare header */
        if (avio_open_dyn_buf(&ctx->pb) < 0) {
            /* XXX: potential leak */
            return -1;
        }
        c->pfmt_ctx->pb->seekable = 0;
        av_write_trailer(ctx);
        len = avio_close_dyn_buf(ctx->pb, &c->pb_buffer);
        c->buffer_ptr = c->pb_buffer;
        c->buffer_end = c->pb_buffer + len;

        c->last_packet_sent = 1;
        break;
    }
    return 0;
}
