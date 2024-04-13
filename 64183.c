    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection *rules = &ofm->old_rules;
    enum ofperr error;

    error = collect_rules_loose(ofproto, &ofm->criteria, rules);

    if (!error) {
        delete_flows_start__(ofproto, ofm->version, rules);
    }

    return error;
}
