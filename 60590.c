static int prepare_sdp_description(FFServerStream *stream, uint8_t **pbuffer,
                                   struct in_addr my_ip)
{
    AVFormatContext *avc;
    AVOutputFormat *rtp_format = av_guess_format("rtp", NULL, NULL);
    AVDictionaryEntry *entry = av_dict_get(stream->metadata, "title", NULL, 0);
    int i;

    *pbuffer = NULL;

    avc =  avformat_alloc_context();
    if (!avc || !rtp_format)
        return -1;

    avc->oformat = rtp_format;
    av_dict_set(&avc->metadata, "title",
                entry ? entry->value : "No Title", 0);
    if (stream->is_multicast) {
        snprintf(avc->filename, 1024, "rtp://%s:%d?multicast=1?ttl=%d",
                 inet_ntoa(stream->multicast_ip),
                 stream->multicast_port, stream->multicast_ttl);
    } else
        snprintf(avc->filename, 1024, "rtp://0.0.0.0");

    for(i = 0; i < stream->nb_streams; i++) {
        AVStream *st = avformat_new_stream(avc, NULL);
        if (!st)
            goto sdp_done;
        avcodec_parameters_from_context(stream->streams[i]->codecpar, stream->streams[i]->codec);
        unlayer_stream(st, stream->streams[i]);
    }
#define PBUFFER_SIZE 2048
    *pbuffer = av_mallocz(PBUFFER_SIZE);
    if (!*pbuffer)
        goto sdp_done;
    av_sdp_create(&avc, 1, *pbuffer, PBUFFER_SIZE);

 sdp_done:
    av_freep(&avc->streams);
    av_dict_free(&avc->metadata);
    av_free(avc);

    return *pbuffer ? strlen(*pbuffer) : AVERROR(ENOMEM);
}
