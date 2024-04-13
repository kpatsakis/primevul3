static int asf_set_metadata(AVFormatContext *s, const uint8_t *name,
                            int type, AVDictionary **met)
{
    AVIOContext *pb = s->pb;
    uint64_t value;
    char buf[32];
    int ret;

    ret = asf_read_generic_value(pb, type, &value);
    if (ret < 0)
        return ret;

    snprintf(buf, sizeof(buf), "%"PRIu64, value);
    if (av_dict_set(met, name, buf, 0) < 0)
        av_log(s, AV_LOG_WARNING, "av_dict_set failed.\n");

    return 0;
}
