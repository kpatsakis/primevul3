    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *old_rule = rule_collection_n(&ofm->old_rules)
        ? rule_collection_rules(&ofm->old_rules)[0] : NULL;
    struct rule *new_rule = rule_collection_rules(&ofm->new_rules)[0];

    replace_rule_revert(ofproto, old_rule, new_rule);
}
