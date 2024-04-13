ofputil_decode_tlv_table_mod(const struct ofp_header *oh,
                                struct ofputil_tlv_table_mod *ttm)
{
    struct ofpbuf msg = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&msg);

    struct nx_tlv_table_mod *nx_ttm = ofpbuf_pull(&msg, sizeof *nx_ttm);
    ttm->command = ntohs(nx_ttm->command);
    if (ttm->command > NXTTMC_CLEAR) {
        VLOG_WARN_RL(&bad_ofmsg_rl,
                     "tlv table mod command (%u) is out of range",
                     ttm->command);
        return OFPERR_NXTTMFC_BAD_COMMAND;
    }

    return decode_tlv_table_mappings(&msg, TUN_METADATA_NUM_OPTS,
                                        &ttm->mappings);
}
