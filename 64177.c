    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection *old_rules = &ofm->old_rules;
    struct rule_collection *new_rules = &ofm->new_rules;

    /* Old rules were not changed yet, only need to revert new rules. */
    if (rule_collection_n(old_rules) > 0) {
        struct rule *old_rule, *new_rule;
        RULE_COLLECTIONS_FOR_EACH (old_rule, new_rule, old_rules, new_rules) {
            replace_rule_revert(ofproto, old_rule, new_rule);
        }
        rule_collection_destroy(new_rules);
        rule_collection_destroy(old_rules);
    }
}
