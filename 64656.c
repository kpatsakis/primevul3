ofputil_put_ofp13_table_stats(const struct ofputil_table_stats *stats,
                              struct ofpbuf *buf)
{
    struct ofp13_table_stats *out;

    out = ofpbuf_put_zeros(buf, sizeof *out);
    out->table_id = stats->table_id;
    out->active_count = htonl(stats->active_count);
    out->lookup_count = htonll(stats->lookup_count);
    out->matched_count = htonll(stats->matched_count);
}
