    OVS_REQUIRES(ofproto_mutex)
{
    if (rule->eviction_group) {
        struct oftable *table = &rule->ofproto->tables[rule->table_id];
        struct eviction_group *evg = rule->eviction_group;

        rule->eviction_group = NULL;
        heap_remove(&evg->rules, &rule->evg_node);
        if (heap_is_empty(&evg->rules)) {
            eviction_group_destroy(table, evg);
        } else {
            eviction_group_resized(table, evg);
        }
    }
}
