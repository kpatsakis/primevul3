handle_nxt_flow_mod_table_id(struct ofconn *ofconn,
                             const struct ofp_header *oh)
{
    const struct nx_flow_mod_table_id *msg = ofpmsg_body(oh);
    enum ofputil_protocol cur, next;

    cur = ofconn_get_protocol(ofconn);
    next = ofputil_protocol_set_tid(cur, msg->set != 0);
    ofconn_set_protocol(ofconn, next);

    return 0;
}
