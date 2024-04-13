ofputil_encode_tlv_table_mod(enum ofp_version ofp_version,
                                struct ofputil_tlv_table_mod *ttm)
{
    struct ofpbuf *b;
    struct nx_tlv_table_mod *nx_ttm;

    b = ofpraw_alloc(OFPRAW_NXT_TLV_TABLE_MOD, ofp_version, 0);
    nx_ttm = ofpbuf_put_zeros(b, sizeof *nx_ttm);
    nx_ttm->command = htons(ttm->command);
    encode_tlv_table_mappings(b, &ttm->mappings);

    return b;
}
