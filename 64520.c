ofputil_decode_table_status(const struct ofp_header *oh,
                            struct ofputil_table_status *ts)
{
    const struct ofp14_table_status *ots;
    struct ofpbuf b;
    enum ofperr error;
    enum ofpraw raw;

    ofpbuf_use_const(&b, oh, ntohs(oh->length));
    raw = ofpraw_pull_assert(&b);
    ots = ofpbuf_pull(&b, sizeof *ots);

    if (raw == OFPRAW_OFPT14_TABLE_STATUS) {
        if (ots->reason != OFPTR_VACANCY_DOWN
            && ots->reason != OFPTR_VACANCY_UP) {
            return OFPERR_OFPBPC_BAD_VALUE;
        }
        ts->reason = ots->reason;

        error = ofputil_decode_table_desc(&b, &ts->desc, oh->version);
        return error;
    } else {
        return OFPERR_OFPBRC_BAD_VERSION;
    }

    return 0;
}
