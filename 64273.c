meter_insert_rule(struct rule *rule)
{
    const struct rule_actions *a = rule_get_actions(rule);
    uint32_t meter_id = ofpacts_get_meter(a->ofpacts, a->ofpacts_len);
    struct meter *meter = rule->ofproto->meters[meter_id];

    ovs_list_insert(&meter->rules, &rule->meter_list_node);
}
