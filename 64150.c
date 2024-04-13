    OVS_EXCLUDED(ofproto_mutex)
{
    enum ofperr error;

    /* Forward flow mod fields we need later. */
    ofm->command = fm->command;
    ofm->modify_cookie = fm->modify_cookie;

    ofm->modify_may_add_flow = (fm->new_cookie != OVS_BE64_MAX
                                && fm->cookie_mask == htonll(0));
    /* Old flags must be kept when modifying a flow, but we still must
     * honor the reset counts flag if present in the flow mod. */
    ofm->modify_keep_counts = !(fm->flags & OFPUTIL_FF_RESET_COUNTS);

    /* Initialize state needed by ofproto_flow_mod_uninit(). */
    ofm->temp_rule = rule;
    ofm->criteria.version = OVS_VERSION_NOT_REMOVED;
    ofm->conjs = NULL;
    ofm->n_conjs = 0;

    bool check_buffer_id = false;

    switch (ofm->command) {
    case OFPFC_ADD:
        check_buffer_id = true;
        error = add_flow_init(ofproto, ofm, fm);
        break;
    case OFPFC_MODIFY:
        check_buffer_id = true;
        error = modify_flows_init_loose(ofproto, ofm, fm);
        break;
    case OFPFC_MODIFY_STRICT:
        check_buffer_id = true;
        error = modify_flow_init_strict(ofproto, ofm, fm);
        break;
    case OFPFC_DELETE:
        error = delete_flows_init_loose(ofproto, ofm, fm);
        break;
    case OFPFC_DELETE_STRICT:
        error = delete_flows_init_strict(ofproto, ofm, fm);
        break;
    default:
        error = OFPERR_OFPFMFC_BAD_COMMAND;
        break;
    }
    if (!error && check_buffer_id && fm->buffer_id != UINT32_MAX) {
        error = OFPERR_OFPBRC_BUFFER_UNKNOWN;
    }

    if (error) {
        ofproto_flow_mod_uninit(ofm);
    }
    return error;
}
