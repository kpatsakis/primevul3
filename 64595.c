ofputil_make_flow_mod_table_id(bool flow_mod_table_id)
{
    struct nx_flow_mod_table_id *nfmti;
    struct ofpbuf *msg;

    msg = ofpraw_alloc(OFPRAW_NXT_FLOW_MOD_TABLE_ID, OFP10_VERSION, 0);
    nfmti = ofpbuf_put_zeros(msg, sizeof *nfmti);
    nfmti->set = flow_mod_table_id;
    return msg;
}
