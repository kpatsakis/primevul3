static int asf_read_content_desc(AVFormatContext *s, const GUIDParseTable *g)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    int i;
    static const char *const titles[] =
    { "Title", "Author", "Copyright", "Description", "Rate" };
    uint16_t len[5], buflen[5] = { 0 };
    uint8_t *ch;
    uint64_t size = avio_rl64(pb);

    for (i = 0; i < 5; i++) {
        len[i]  = avio_rl16(pb);
        buflen[i]  = 2 * len[i] + 1;
    }

    for (i = 0; i < 5; i++) {
        ch = av_malloc(buflen[i]);
        if (!ch)
            return(AVERROR(ENOMEM));
        asf_read_metadata(s, titles[i], len[i], ch, buflen[i]);
        av_freep(&ch);
    }
    align_position(pb, asf->offset, size);

    return 0;
}
