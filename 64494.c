ofputil_decode_ofp12_table_stats(struct ofpbuf *msg,
                                 struct ofputil_table_stats *stats,
                                 struct ofputil_table_features *features)
{
    struct ofp12_table_stats *ots;

    ots = ofpbuf_try_pull(msg, sizeof *ots);
    if (!ots) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    features->table_id = ots->table_id;
    ovs_strlcpy(features->name, ots->name, sizeof features->name);
    features->metadata_match = ots->metadata_match;
    features->metadata_write = ots->metadata_write;
    features->miss_config = ofputil_decode_table_miss(ots->config,
                                                      OFP12_VERSION);
    features->max_entries = ntohl(ots->max_entries);

    features->nonmiss.instructions = ovsinst_bitmap_from_openflow(
        ots->instructions, OFP12_VERSION);
    features->nonmiss.write.ofpacts = ofpact_bitmap_from_openflow(
        ots->write_actions, OFP12_VERSION);
    features->nonmiss.apply.ofpacts = ofpact_bitmap_from_openflow(
        ots->apply_actions, OFP12_VERSION);
    features->nonmiss.write.set_fields = oxm_bitmap_to_mf_bitmap(
        ots->write_setfields, OFP12_VERSION);
    features->nonmiss.apply.set_fields = oxm_bitmap_to_mf_bitmap(
        ots->apply_setfields, OFP12_VERSION);
    features->miss = features->nonmiss;

    features->match = oxm_bitmap_to_mf_bitmap(ots->match, OFP12_VERSION);
    features->wildcard = oxm_bitmap_to_mf_bitmap(ots->wildcards,
                                                 OFP12_VERSION);
    bitmap_or(features->match.bm, features->wildcard.bm, MFF_N_IDS);

    stats->table_id = ots->table_id;
    stats->active_count = ntohl(ots->active_count);
    stats->lookup_count = ntohll(ots->lookup_count);
    stats->matched_count = ntohll(ots->matched_count);

    return 0;
}
