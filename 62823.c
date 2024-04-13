static int asf_read_metadata(AVFormatContext *s, const char *title, uint16_t len,
                             unsigned char *ch, uint16_t buflen)
{
    AVIOContext *pb = s->pb;

    avio_get_str16le(pb, len, ch, buflen);
    if (ch[0]) {
        if (av_dict_set(&s->metadata, title, ch, 0) < 0)
            av_log(s, AV_LOG_WARNING, "av_dict_set failed.\n");
    }

    return 0;
}
