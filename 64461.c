ofputil_count_ipfix_stats(const struct ofp_header *oh)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&b);

    return b.size / sizeof(struct ofputil_ipfix_stats);
}
