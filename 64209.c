    OVS_REQUIRES(ofproto_mutex)
{
    while (!heap_is_empty(&evg->rules)) {
        struct rule *rule;

        rule = CONTAINER_OF(heap_pop(&evg->rules), struct rule, evg_node);
        rule->eviction_group = NULL;
    }
    hmap_remove(&table->eviction_groups_by_id, &evg->id_node);
    heap_remove(&table->eviction_groups_by_size, &evg->size_node);
    heap_destroy(&evg->rules);
    free(evg);
}
