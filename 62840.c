static int asf_store_aspect_ratio(AVFormatContext *s, uint8_t st_num, uint8_t *name, int type)
{
    ASFContext *asf   = s->priv_data;
    AVIOContext *pb   = s->pb;
    uint64_t value = 0;
    int ret;

    ret = asf_read_generic_value(pb, type, &value);
    if (ret < 0)
        return ret;

    if (st_num < ASF_MAX_STREAMS) {
        if (!strcmp(name, "AspectRatioX"))
            asf->asf_sd[st_num].aspect_ratio.num = value;
        else
            asf->asf_sd[st_num].aspect_ratio.den = value;
    }
    return 0;
}
