replace_rule_revert(struct ofproto *ofproto,
                    struct rule *old_rule, struct rule *new_rule)
{
    struct oftable *table = &ofproto->tables[new_rule->table_id];

    if (old_rule) {
        if (old_rule->removed_reason == OFPRR_EVICTION) {
            /* Revert the eviction. */
            eviction_group_add_rule(old_rule);
        }

        /* Restore the old rule to data structures. */
        ofproto_rule_insert__(ofproto, old_rule);

        /* Restore the original visibility of the old rule. */
        cls_rule_restore_visibility(&old_rule->cr);
    } else {
        /* Restore table's rule count. */
        table->n_flows--;
    }

    /* Remove the new rule immediately.  It was never visible to lookups. */
    if (!classifier_remove(&table->cls, &new_rule->cr)) {
        OVS_NOT_REACHED();
    }
    ofproto_rule_remove__(ofproto, new_rule);
    ofproto_rule_unref(new_rule);
}
