static av_cold int ac3_parse_init(AVCodecParserContext *s1)
{
    AACAC3ParseContext *s = s1->priv_data;
    s->header_size = AC3_HEADER_SIZE;
    s->sync = ac3_sync;
    return 0;
}
