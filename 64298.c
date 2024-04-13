ofproto_configure_table(struct ofproto *ofproto, int table_id,
                        const struct ofproto_table_settings *s)
{
    struct oftable *table;

    ovs_assert(table_id >= 0 && table_id < ofproto->n_tables);
    table = &ofproto->tables[table_id];

    oftable_set_name(table, s->name);

    if (table->flags & OFTABLE_READONLY) {
        return;
    }

    if (classifier_set_prefix_fields(&table->cls,
                                     s->prefix_fields, s->n_prefix_fields)) {
        /* XXX: Trigger revalidation. */
    }

    ovs_mutex_lock(&ofproto_mutex);
    unsigned int new_eviction = (s->enable_eviction
                                 ? table->eviction | EVICTION_CLIENT
                                 : table->eviction & ~EVICTION_CLIENT);
    oftable_configure_eviction(table, new_eviction, s->groups, s->n_groups);
    table->max_flows = s->max_flows;
    evict_rules_from_table(table);
    ovs_mutex_unlock(&ofproto_mutex);
}
