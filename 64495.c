ofputil_decode_ofp13_table_stats(struct ofpbuf *msg,
                                 struct ofputil_table_stats *stats,
                                 struct ofputil_table_features *features)
{
    struct ofp13_table_stats *ots;

    ots = ofpbuf_try_pull(msg, sizeof *ots);
    if (!ots) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    features->table_id = ots->table_id;

    stats->table_id = ots->table_id;
    stats->active_count = ntohl(ots->active_count);
    stats->lookup_count = ntohll(ots->lookup_count);
    stats->matched_count = ntohll(ots->matched_count);

    return 0;
}
