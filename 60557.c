static void compute_bandwidth(void)
{
    unsigned bandwidth;
    int i;
    FFServerStream *stream;

    for(stream = config.first_stream; stream; stream = stream->next) {
        bandwidth = 0;
        for(i=0;i<stream->nb_streams;i++) {
            LayeredAVStream *st = stream->streams[i];
            switch(st->codec->codec_type) {
            case AVMEDIA_TYPE_AUDIO:
            case AVMEDIA_TYPE_VIDEO:
                bandwidth += st->codec->bit_rate;
                break;
            default:
                break;
            }
        }
        stream->bandwidth = (bandwidth + 999) / 1000;
    }
}
