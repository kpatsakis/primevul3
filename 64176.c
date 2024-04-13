    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection *old_rules = &ofm->old_rules;
    enum ofperr error;

    error = collect_rules_loose(ofproto, &ofm->criteria, old_rules);

    if (!error) {
        error = modify_flows_start__(ofproto, ofm);
    }

    if (error) {
        rule_collection_destroy(old_rules);
    }

    return error;
}
