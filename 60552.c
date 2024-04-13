static int build_feed_streams(void)
{
    FFServerStream *stream, *feed;
    int i, fd;

    /* gather all streams */
    for(stream = config.first_stream; stream; stream = stream->next) {
        feed = stream->feed;
        if (!feed)
            continue;

        if (stream->is_feed) {
            for(i=0;i<stream->nb_streams;i++)
                stream->feed_streams[i] = i;
            continue;
        }
        /* we handle a stream coming from a feed */
        for(i=0;i<stream->nb_streams;i++)
            stream->feed_streams[i] = add_av_stream(feed, stream->streams[i]);
    }

    /* create feed files if needed */
    for(feed = config.first_feed; feed; feed = feed->next_feed) {

        if (avio_check(feed->feed_filename, AVIO_FLAG_READ) > 0) {
            AVFormatContext *s = NULL;
            int matches = 0;

            /* See if it matches */

            if (avformat_open_input(&s, feed->feed_filename, NULL, NULL) < 0) {
                http_log("Deleting feed file '%s' as it appears "
                            "to be corrupt\n",
                         feed->feed_filename);
                goto drop;
            }

            /* set buffer size */
            if (ffio_set_buf_size(s->pb, FFM_PACKET_SIZE) < 0) {
                http_log("Failed to set buffer size\n");
                avformat_close_input(&s);
                goto bail;
            }

            /* Now see if it matches */
            if (s->nb_streams != feed->nb_streams) {
                http_log("Deleting feed file '%s' as stream counts "
                            "differ (%d != %d)\n",
                         feed->feed_filename, s->nb_streams, feed->nb_streams);
                goto drop;
            }

            matches = 1;
            for(i=0;i<s->nb_streams;i++) {
                AVStream *ss;
                LayeredAVStream *sf;

                sf = feed->streams[i];
                ss = s->streams[i];

                if (sf->index != ss->index || sf->id != ss->id) {
                    http_log("Index & Id do not match for stream %d (%s)\n",
                             i, feed->feed_filename);
                    matches = 0;
                    break;
                }

                matches = check_codec_match (sf, ss, i);
                if (!matches)
                    break;
            }

drop:
            if (s)
                avformat_close_input(&s);

            if (!matches) {
                if (feed->readonly) {
                    http_log("Unable to delete read-only feed file '%s'\n",
                             feed->feed_filename);
                    goto bail;
                }
                unlink(feed->feed_filename);
            }
        }

        if (avio_check(feed->feed_filename, AVIO_FLAG_WRITE) <= 0) {
            AVFormatContext *s = avformat_alloc_context();

            if (!s) {
                http_log("Failed to allocate context\n");
                goto bail;
            }

            if (feed->readonly) {
                http_log("Unable to create feed file '%s' as it is "
                            "marked readonly\n",
                         feed->feed_filename);
                avformat_free_context(s);
                goto bail;
            }

            /* only write the header of the ffm file */
            if (avio_open(&s->pb, feed->feed_filename, AVIO_FLAG_WRITE) < 0) {
                http_log("Could not open output feed file '%s'\n",
                         feed->feed_filename);
                avformat_free_context(s);
                goto bail;
            }
            s->oformat = feed->fmt;
            for (i = 0; i<feed->nb_streams; i++) {
                AVStream *st = avformat_new_stream(s, NULL); // FIXME free this
                if (!st) {
                    http_log("Failed to allocate stream\n");
                    goto bail;
                }
                unlayer_stream(st, feed->streams[i]);
            }
            if (avformat_write_header(s, NULL) < 0) {
                http_log("Container doesn't support the required parameters\n");
                avio_closep(&s->pb);
                s->streams = NULL;
                s->nb_streams = 0;
                avformat_free_context(s);
                goto bail;
            }
            /* XXX: need better API */
            av_freep(&s->priv_data);
            avio_closep(&s->pb);
            s->streams = NULL;
            s->nb_streams = 0;
            avformat_free_context(s);
        }

        /* get feed size and write index */
        fd = open(feed->feed_filename, O_RDONLY);
        if (fd < 0) {
            http_log("Could not open output feed file '%s'\n",
                    feed->feed_filename);
            goto bail;
        }

        feed->feed_write_index = FFMAX(ffm_read_write_index(fd),
                                       FFM_PACKET_SIZE);
        feed->feed_size = lseek(fd, 0, SEEK_END);
        /* ensure that we do not wrap before the end of file */
        if (feed->feed_max_size && feed->feed_max_size < feed->feed_size)
            feed->feed_max_size = feed->feed_size;

        close(fd);
    }
    return 0;

bail:
    return -1;
}
