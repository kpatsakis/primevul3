static int add_av_stream(FFServerStream *feed, LayeredAVStream *st)
{
    LayeredAVStream *fst;
    AVCodecContext *av, *av1;
    int i;

    av = st->codec;
    for(i=0;i<feed->nb_streams;i++) {
        av1 = feed->streams[i]->codec;
        if (av1->codec_id == av->codec_id &&
            av1->codec_type == av->codec_type &&
            av1->bit_rate == av->bit_rate) {

            switch(av->codec_type) {
            case AVMEDIA_TYPE_AUDIO:
                if (av1->channels == av->channels &&
                    av1->sample_rate == av->sample_rate)
                    return i;
                break;
            case AVMEDIA_TYPE_VIDEO:
                if (av1->width == av->width &&
                    av1->height == av->height &&
                    av1->time_base.den == av->time_base.den &&
                    av1->time_base.num == av->time_base.num &&
                    av1->gop_size == av->gop_size)
                    return i;
                break;
            default:
                abort();
            }
        }
    }

    fst = add_av_stream1(feed, av, 0);
    if (!fst)
        return -1;
    if (st->recommended_encoder_configuration)
        fst->recommended_encoder_configuration =
            av_strdup(st->recommended_encoder_configuration);
    return feed->nb_streams - 1;
}
