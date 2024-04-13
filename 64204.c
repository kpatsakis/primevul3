    OVS_REQUIRES(ofproto_mutex)
{
    enum ofperr error;

    rule_collection_init(&ofm->old_rules);
    rule_collection_init(&ofm->new_rules);

    switch (ofm->command) {
    case OFPFC_ADD:
        error = add_flow_start(ofproto, ofm);
        break;
    case OFPFC_MODIFY:
        error = modify_flows_start_loose(ofproto, ofm);
        break;
    case OFPFC_MODIFY_STRICT:
        error = modify_flow_start_strict(ofproto, ofm);
        break;
    case OFPFC_DELETE:
        error = delete_flows_start_loose(ofproto, ofm);
        break;
    case OFPFC_DELETE_STRICT:
        error = delete_flow_start_strict(ofproto, ofm);
        break;
    default:
        OVS_NOT_REACHED();
    }
    /* Release resources not needed after start. */
    ofproto_flow_mod_uninit(ofm);

    if (error) {
        rule_collection_destroy(&ofm->old_rules);
        rule_collection_destroy(&ofm->new_rules);
    }
    return error;
}
