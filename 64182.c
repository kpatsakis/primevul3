    OVS_REQUIRES(ofproto_mutex)
{
    if (rule_collection_n(rules)) {
        struct ofproto *ofproto = rule_collection_rules(rules)[0]->ofproto;

        delete_flows_start__(ofproto, ofproto->tables_version + 1, rules);
        ofproto_bump_tables_version(ofproto);
        delete_flows_finish__(ofproto, rules, reason, req);
        ofmonitor_flush(ofproto->connmgr);
    }
}
