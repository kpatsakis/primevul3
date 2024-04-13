static int flv_write_header(AVFormatContext *s)
{
    int i;
    AVIOContext *pb = s->pb;
    FLVContext *flv = s->priv_data;

    for (i = 0; i < s->nb_streams; i++) {
        AVCodecParameters *par = s->streams[i]->codecpar;
        FLVStreamContext *sc;
        switch (par->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
            if (s->streams[i]->avg_frame_rate.den &&
                s->streams[i]->avg_frame_rate.num) {
                flv->framerate = av_q2d(s->streams[i]->avg_frame_rate);
            }
            if (flv->video_par) {
                av_log(s, AV_LOG_ERROR,
                       "at most one video stream is supported in flv\n");
                return AVERROR(EINVAL);
            }
            flv->video_par = par;
            if (!ff_codec_get_tag(flv_video_codec_ids, par->codec_id))
                return unsupported_codec(s, "Video", par->codec_id);

            if (par->codec_id == AV_CODEC_ID_MPEG4 ||
                par->codec_id == AV_CODEC_ID_H263) {
                int error = s->strict_std_compliance > FF_COMPLIANCE_UNOFFICIAL;
                av_log(s, error ? AV_LOG_ERROR : AV_LOG_WARNING,
                       "Codec %s is not supported in the official FLV specification,\n", avcodec_get_name(par->codec_id));

                if (error) {
                    av_log(s, AV_LOG_ERROR,
                           "use vstrict=-1 / -strict -1 to use it anyway.\n");
                    return AVERROR(EINVAL);
                }
            } else if (par->codec_id == AV_CODEC_ID_VP6) {
                av_log(s, AV_LOG_WARNING,
                       "Muxing VP6 in flv will produce flipped video on playback.\n");
            }
            break;
        case AVMEDIA_TYPE_AUDIO:
            if (flv->audio_par) {
                av_log(s, AV_LOG_ERROR,
                       "at most one audio stream is supported in flv\n");
                return AVERROR(EINVAL);
            }
            flv->audio_par = par;
            if (get_audio_flags(s, par) < 0)
                return unsupported_codec(s, "Audio", par->codec_id);
            if (par->codec_id == AV_CODEC_ID_PCM_S16BE)
                av_log(s, AV_LOG_WARNING,
                       "16-bit big-endian audio in flv is valid but most likely unplayable (hardware dependent); use s16le\n");
            break;
        case AVMEDIA_TYPE_DATA:
            if (par->codec_id != AV_CODEC_ID_TEXT && par->codec_id != AV_CODEC_ID_NONE)
                return unsupported_codec(s, "Data", par->codec_id);
            flv->data_par = par;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            if (par->codec_id != AV_CODEC_ID_TEXT) {
                av_log(s, AV_LOG_ERROR, "Subtitle codec '%s' for stream %d is not compatible with FLV\n",
                       avcodec_get_name(par->codec_id), i);
                return AVERROR_INVALIDDATA;
            }
            flv->data_par = par;
            break;
        default:
            av_log(s, AV_LOG_ERROR, "Codec type '%s' for stream %d is not compatible with FLV\n",
                   av_get_media_type_string(par->codec_type), i);
            return AVERROR(EINVAL);
        }
        avpriv_set_pts_info(s->streams[i], 32, 1, 1000); /* 32 bit pts in ms */

        sc = av_mallocz(sizeof(FLVStreamContext));
        if (!sc)
            return AVERROR(ENOMEM);
        s->streams[i]->priv_data = sc;
        sc->last_ts = -1;
    }

    flv->delay = AV_NOPTS_VALUE;

    avio_write(pb, "FLV", 3);
    avio_w8(pb, 1);
    avio_w8(pb, FLV_HEADER_FLAG_HASAUDIO * !!flv->audio_par +
                FLV_HEADER_FLAG_HASVIDEO * !!flv->video_par);
    avio_wb32(pb, 9);
    avio_wb32(pb, 0);

    for (i = 0; i < s->nb_streams; i++)
        if (s->streams[i]->codecpar->codec_tag == 5) {
            avio_w8(pb, 8);     // message type
            avio_wb24(pb, 0);   // include flags
            avio_wb24(pb, 0);   // time stamp
            avio_wb32(pb, 0);   // reserved
            avio_wb32(pb, 11);  // size
            flv->reserved = 5;
        }

    if (flv->flags & FLV_NO_METADATA) {
        pb->seekable = 0;
    } else {
        write_metadata(s, 0);
    }

    for (i = 0; i < s->nb_streams; i++) {
        flv_write_codec_header(s, s->streams[i]->codecpar, 0);
    }

    flv->datastart_offset = avio_tell(pb);
    return 0;
}
