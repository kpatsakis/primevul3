static inline void print_stream_params(AVIOContext *pb, FFServerStream *stream)
{
    int i, stream_no;
    const char *type = "unknown";
    char parameters[64];
    LayeredAVStream *st;
    AVCodec *codec;

    stream_no = stream->nb_streams;

    avio_printf(pb, "<table><tr><th>Stream<th>"
                    "type<th>kbit/s<th>codec<th>"
                    "Parameters\n");

    for (i = 0; i < stream_no; i++) {
        st = stream->streams[i];
        codec = avcodec_find_encoder(st->codecpar->codec_id);

        parameters[0] = 0;

        switch(st->codecpar->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            type = "audio";
            snprintf(parameters, sizeof(parameters), "%d channel(s), %d Hz",
                     st->codecpar->channels, st->codecpar->sample_rate);
            break;
        case AVMEDIA_TYPE_VIDEO:
            type = "video";
            snprintf(parameters, sizeof(parameters),
                     "%dx%d, q=%d-%d, fps=%d", st->codecpar->width,
                     st->codecpar->height, st->codec->qmin, st->codec->qmax,
                     st->time_base.den / st->time_base.num);
            break;
        default:
            abort();
        }

        avio_printf(pb, "<tr><td>%d<td>%s<td>%"PRId64
                        "<td>%s<td>%s\n",
                    i, type, (int64_t)st->codecpar->bit_rate/1000,
                    codec ? codec->name : "", parameters);
     }

     avio_printf(pb, "</table>\n");
}
