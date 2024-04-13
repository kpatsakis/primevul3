static int asf_read_close(AVFormatContext *s)
{
    ASFContext *asf = s->priv_data;
    int i;

    for (i = 0; i < ASF_MAX_STREAMS; i++) {
        av_dict_free(&asf->asf_sd[i].asf_met);
        if (i < asf->nb_streams) {
            av_packet_unref(&asf->asf_st[i]->pkt.avpkt);
            av_freep(&asf->asf_st[i]);
        }
    }

    asf->nb_streams = 0;
    return 0;
}
