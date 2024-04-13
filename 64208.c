    OVS_REQUIRES(ofproto_mutex)
{
    heap_change(&table->eviction_groups_by_size, &evg->size_node,
                eviction_group_priority(heap_count(&evg->rules)));
}
