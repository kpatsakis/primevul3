ofputil_decode_tlv_table_reply(const struct ofp_header *oh,
                                  struct ofputil_tlv_table_reply *ttr)
{
    struct ofpbuf msg = ofpbuf_const_initializer(oh, ntohs(oh->length));
    ofpraw_pull_assert(&msg);

    struct nx_tlv_table_reply *nx_ttr = ofpbuf_pull(&msg, sizeof *nx_ttr);
    ttr->max_option_space = ntohl(nx_ttr->max_option_space);
    ttr->max_fields = ntohs(nx_ttr->max_fields);

    return decode_tlv_table_mappings(&msg, ttr->max_fields, &ttr->mappings);
}
