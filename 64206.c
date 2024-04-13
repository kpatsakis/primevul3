    OVS_REQUIRES(ofproto_mutex)
{
    switch (ofm->command) {
    case OFPFC_ADD:
        add_flow_finish(ofproto, ofm, req);
        break;

    case OFPFC_MODIFY:
    case OFPFC_MODIFY_STRICT:
        modify_flows_finish(ofproto, ofm, req);
        break;

    case OFPFC_DELETE:
    case OFPFC_DELETE_STRICT:
        delete_flows_finish(ofproto, ofm, req);
        break;

    default:
        break;
    }

    rule_collection_destroy(&ofm->old_rules);
    rule_collection_destroy(&ofm->new_rules);

    if (req) {
        ofconn_report_flow_mod(req->ofconn, ofm->command);
    }
}
