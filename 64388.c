oftable_destroy(struct oftable *table)
{
    ovs_assert(classifier_is_empty(&table->cls));

    ovs_mutex_lock(&ofproto_mutex);
    oftable_configure_eviction(table, 0, NULL, 0);
    ovs_mutex_unlock(&ofproto_mutex);

    hmap_destroy(&table->eviction_groups_by_id);
    heap_destroy(&table->eviction_groups_by_size);
    classifier_destroy(&table->cls);
    free(table->name);
}
