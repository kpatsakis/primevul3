    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule;

    RULE_COLLECTION_FOR_EACH (rule, rules) {
        struct oftable *table = &ofproto->tables[rule->table_id];

        /* Add rule back to ofproto data structures. */
        ofproto_rule_insert__(ofproto, rule);

        /* Restore table's rule count. */
        table->n_flows++;

        /* Restore the original visibility of the rule. */
        cls_rule_restore_visibility(&rule->cr);
    }
}
