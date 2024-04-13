    OVS_EXCLUDED(ofproto_mutex)
{
    struct rule **orig_rules = rules;

    if (*rules) {
        struct ofproto *ofproto = rules[0]->ofproto;
        unsigned long tables[BITMAP_N_LONGS(256)];
        struct rule *rule;
        size_t table_id;

        memset(tables, 0, sizeof tables);

        ovs_mutex_lock(&ofproto_mutex);
        while ((rule = *rules++)) {
            /* Defer once for each new table.  This defers the subtable cleanup
             * until later, so that when removing large number of flows the
             * operation is faster. */
            if (!bitmap_is_set(tables, rule->table_id)) {
                struct classifier *cls = &ofproto->tables[rule->table_id].cls;

                bitmap_set1(tables, rule->table_id);
                classifier_defer(cls);
            }
            remove_rule_rcu__(rule);
        }

        BITMAP_FOR_EACH_1(table_id, 256, tables) {
            struct classifier *cls = &ofproto->tables[table_id].cls;

            classifier_publish(cls);
        }
        ovs_mutex_unlock(&ofproto_mutex);
    }

    free(orig_rules);
}
