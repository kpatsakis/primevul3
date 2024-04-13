ofputil_put_ofp12_table_stats(const struct ofputil_table_stats *stats,
                              const struct ofputil_table_features *features,
                              struct ofpbuf *buf)
{
    struct ofp12_table_stats *out;

    out = ofpbuf_put_zeros(buf, sizeof *out);
    out->table_id = features->table_id;
    ovs_strlcpy(out->name, features->name, sizeof out->name);
    out->match = oxm_bitmap_from_mf_bitmap(&features->match, OFP12_VERSION);
    out->wildcards = oxm_bitmap_from_mf_bitmap(&features->wildcard,
                                             OFP12_VERSION);
    out->write_actions = ofpact_bitmap_to_openflow(
        features->nonmiss.write.ofpacts, OFP12_VERSION);
    out->apply_actions = ofpact_bitmap_to_openflow(
        features->nonmiss.apply.ofpacts, OFP12_VERSION);
    out->write_setfields = oxm_bitmap_from_mf_bitmap(
        &features->nonmiss.write.set_fields, OFP12_VERSION);
    out->apply_setfields = oxm_bitmap_from_mf_bitmap(
        &features->nonmiss.apply.set_fields, OFP12_VERSION);
    out->metadata_match = features->metadata_match;
    out->metadata_write = features->metadata_write;
    out->instructions = ovsinst_bitmap_to_openflow(
        features->nonmiss.instructions, OFP12_VERSION);
    out->config = ofputil_encode_table_config(features->miss_config,
                                              OFPUTIL_TABLE_EVICTION_DEFAULT,
                                              OFPUTIL_TABLE_VACANCY_DEFAULT,
                                              OFP12_VERSION);
    out->max_entries = htonl(features->max_entries);
    out->active_count = htonl(stats->active_count);
    out->lookup_count = htonll(stats->lookup_count);
    out->matched_count = htonll(stats->matched_count);
}
