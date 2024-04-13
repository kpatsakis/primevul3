    OVS_REQUIRES(ofproto_mutex)
{
    struct rule *rule;

    if ((table->eviction != 0) == (eviction != 0)
        && n_fields == table->n_eviction_fields
        && (!n_fields
            || !memcmp(fields, table->eviction_fields,
                       n_fields * sizeof *fields))) {
        /* The set of eviction fields did not change.  If 'eviction' changed,
         * it remains nonzero, so that we can just update table->eviction
         * without fussing with the eviction groups. */
        table->eviction = eviction;
        return;
    }

    /* Destroy existing eviction groups, then destroy and recreate data
     * structures to recover memory. */
    struct eviction_group *evg, *next;
    HMAP_FOR_EACH_SAFE (evg, next, id_node, &table->eviction_groups_by_id) {
        eviction_group_destroy(table, evg);
    }
    hmap_destroy(&table->eviction_groups_by_id);
    hmap_init(&table->eviction_groups_by_id);
    heap_destroy(&table->eviction_groups_by_size);
    heap_init(&table->eviction_groups_by_size);

    /* Replace eviction groups by the new ones, if there is a change.  Free the
     * old fields only after allocating the new ones, because 'fields ==
     * table->eviction_fields' is possible. */
    struct mf_subfield *old_fields = table->eviction_fields;
    table->n_eviction_fields = n_fields;
    table->eviction_fields = (fields
                              ? xmemdup(fields, n_fields * sizeof *fields)
                              : NULL);
    free(old_fields);

    /* Add the new eviction groups, if enabled. */
    table->eviction = eviction;
    if (table->eviction) {
        table->eviction_group_id_basis = random_uint32();
        CLS_FOR_EACH (rule, cr, &table->cls) {
            eviction_group_add_rule(rule);
        }
    }
}
