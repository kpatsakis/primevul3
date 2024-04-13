ofputil_count_port_stats(const struct ofp_header *oh)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&b);

    for (size_t n = 0; ; n++) {
        struct ofputil_port_stats ps;
        if (ofputil_decode_port_stats(&ps, &b)) {
            return n;
        }
    }
}
