    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection *old_rules = &ofm->old_rules;
    struct rule_collection *new_rules = &ofm->new_rules;

    if (rule_collection_n(old_rules) == 0
        && rule_collection_n(new_rules) == 1) {
        add_flow_finish(ofproto, ofm, req);
    } else if (rule_collection_n(old_rules) > 0) {
        struct ovs_list dead_cookies = OVS_LIST_INITIALIZER(&dead_cookies);

        ovs_assert(rule_collection_n(new_rules)
                   == rule_collection_n(old_rules));

        struct rule *old_rule, *new_rule;
        RULE_COLLECTIONS_FOR_EACH (old_rule, new_rule, old_rules, new_rules) {
            replace_rule_finish(ofproto, ofm, req, old_rule, new_rule,
                                &dead_cookies);
        }
        learned_cookies_flush(ofproto, &dead_cookies);
        remove_rules_postponed(old_rules);
    }
}
