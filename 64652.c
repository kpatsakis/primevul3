ofputil_put_ofp10_table_stats(const struct ofputil_table_stats *stats,
                              const struct ofputil_table_features *features,
                              struct ofpbuf *buf)
{
    struct mf_bitmap wc = wild_or_nonmatchable_fields(features);
    struct ofp10_table_stats *out;

    out = ofpbuf_put_zeros(buf, sizeof *out);
    out->table_id = features->table_id;
    ovs_strlcpy(out->name, features->name, sizeof out->name);
    out->wildcards = mf_bitmap_to_of10(&wc);
    out->max_entries = htonl(features->max_entries);
    out->active_count = htonl(stats->active_count);
    put_32aligned_be64(&out->lookup_count, htonll(stats->lookup_count));
    put_32aligned_be64(&out->matched_count, htonll(stats->matched_count));
}
