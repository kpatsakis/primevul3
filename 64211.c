    OVS_REQUIRES(ofproto_mutex)
{
    struct eviction_group *evg;

    HMAP_FOR_EACH_WITH_HASH (evg, id_node, id, &table->eviction_groups_by_id) {
        return evg;
    }

    evg = xmalloc(sizeof *evg);
    hmap_insert(&table->eviction_groups_by_id, &evg->id_node, id);
    heap_insert(&table->eviction_groups_by_size, &evg->size_node,
                eviction_group_priority(0));
    heap_init(&evg->rules);

    return evg;
}
