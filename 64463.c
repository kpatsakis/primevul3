ofputil_count_queue_stats(const struct ofp_header *oh)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&b);

    for (size_t n = 0; ; n++) {
        struct ofputil_queue_stats qs;
        if (ofputil_decode_queue_stats(&qs, &b)) {
            return n;
        }
    }
}
