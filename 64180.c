    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule;

    RULE_COLLECTION_FOR_EACH (rule, rules) {
        struct oftable *table = &ofproto->tables[rule->table_id];

        table->n_flows--;
        cls_rule_make_invisible_in_version(&rule->cr, version);

        /* Remove rule from ofproto data structures. */
        ofproto_rule_remove__(ofproto, rule);
    }
}
