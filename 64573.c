ofputil_encode_table_status(const struct ofputil_table_status *ts,
                            enum ofputil_protocol protocol)
{
    enum ofp_version version;
    struct ofpbuf *b;

    version = ofputil_protocol_to_ofp_version(protocol);
    if (version >= OFP14_VERSION) {
        enum ofpraw raw;
        struct ofp14_table_status *ots;

        raw = OFPRAW_OFPT14_TABLE_STATUS;
        b = ofpraw_alloc_xid(raw, version, htonl(0), 0);
        ots = ofpbuf_put_zeros(b, sizeof *ots);
        ots->reason = ts->reason;
        ofputil_put_ofp14_table_desc(&ts->desc, b, version);
        ofpmsg_update_length(b);
        return b;
    } else {
        return NULL;
    }
}
