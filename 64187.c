    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection rules;

    rule_collection_init(&rules);
    rule_collection_add(&rules, rule);
    delete_flows__(&rules, reason, NULL);
}
