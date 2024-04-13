ofputil_decode_ofp10_table_stats(struct ofpbuf *msg,
                                 struct ofputil_table_stats *stats,
                                 struct ofputil_table_features *features)
{
    struct ofp10_table_stats *ots;

    ots = ofpbuf_try_pull(msg, sizeof *ots);
    if (!ots) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    features->table_id = ots->table_id;
    ovs_strlcpy(features->name, ots->name, sizeof features->name);
    features->max_entries = ntohl(ots->max_entries);
    features->match = features->wildcard = mf_bitmap_from_of10(ots->wildcards);

    stats->table_id = ots->table_id;
    stats->active_count = ntohl(ots->active_count);
    stats->lookup_count = ntohll(get_32aligned_be64(&ots->lookup_count));
    stats->matched_count = ntohll(get_32aligned_be64(&ots->matched_count));

    return 0;
}
