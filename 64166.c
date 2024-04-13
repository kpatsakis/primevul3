    OVS_REQUIRES(ofproto_mutex)
{
    if (rule_collection_n(rules) > 0) {
        if (rule_collection_n(rules) == 1) {
            ovsrcu_postpone(remove_rule_rcu, rule_collection_rules(rules)[0]);
            rule_collection_init(rules);
        } else {
            ovsrcu_postpone(remove_rules_rcu, rule_collection_detach(rules));
        }
    }
}
