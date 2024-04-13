oftable_init(struct oftable *table)
{
    memset(table, 0, sizeof *table);
    classifier_init(&table->cls, flow_segment_u64s);
    table->max_flows = UINT_MAX;
    table->n_flows = 0;
    hmap_init(&table->eviction_groups_by_id);
    heap_init(&table->eviction_groups_by_size);
    atomic_init(&table->miss_config, OFPUTIL_TABLE_MISS_DEFAULT);

    classifier_set_prefix_fields(&table->cls, default_prefix_fields,
                                 ARRAY_SIZE(default_prefix_fields));

    atomic_init(&table->n_matched, 0);
    atomic_init(&table->n_missed, 0);
}
