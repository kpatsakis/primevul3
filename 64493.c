ofputil_decode_ofp11_table_stats(struct ofpbuf *msg,
                                 struct ofputil_table_stats *stats,
                                 struct ofputil_table_features *features)
{
    struct ofp11_table_stats *ots;

    ots = ofpbuf_try_pull(msg, sizeof *ots);
    if (!ots) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    features->table_id = ots->table_id;
    ovs_strlcpy(features->name, ots->name, sizeof features->name);
    features->max_entries = ntohl(ots->max_entries);
    features->nonmiss.instructions = ovsinst_bitmap_from_openflow(
        ots->instructions, OFP11_VERSION);
    features->nonmiss.write.ofpacts = ofpact_bitmap_from_openflow(
        ots->write_actions, OFP11_VERSION);
    features->nonmiss.apply.ofpacts = ofpact_bitmap_from_openflow(
        ots->write_actions, OFP11_VERSION);
    features->miss = features->nonmiss;
    features->miss_config = ofputil_decode_table_miss(ots->config,
                                                      OFP11_VERSION);
    features->match = mf_bitmap_from_of11(ots->match);
    features->wildcard = mf_bitmap_from_of11(ots->wildcards);
    bitmap_or(features->match.bm, features->wildcard.bm, MFF_N_IDS);

    stats->table_id = ots->table_id;
    stats->active_count = ntohl(ots->active_count);
    stats->lookup_count = ntohll(ots->lookup_count);
    stats->matched_count = ntohll(ots->matched_count);

    return 0;
}
