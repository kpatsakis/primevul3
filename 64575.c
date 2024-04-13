ofputil_encode_tlv_table_reply(const struct ofp_header *oh,
                                  struct ofputil_tlv_table_reply *ttr)
{
    struct ofpbuf *b;
    struct nx_tlv_table_reply *nx_ttr;

    b = ofpraw_alloc_reply(OFPRAW_NXT_TLV_TABLE_REPLY, oh, 0);
    nx_ttr = ofpbuf_put_zeros(b, sizeof *nx_ttr);
    nx_ttr->max_option_space = htonl(ttr->max_option_space);
    nx_ttr->max_fields = htons(ttr->max_fields);

    encode_tlv_table_mappings(b, &ttr->mappings);

    return b;
}
