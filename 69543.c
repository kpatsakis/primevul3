int blk_mq_alloc_tag_set(struct blk_mq_tag_set *set)
{
	BUILD_BUG_ON(BLK_MQ_MAX_DEPTH > 1 << BLK_MQ_UNIQUE_TAG_BITS);

	if (!set->nr_hw_queues)
		return -EINVAL;
	if (!set->queue_depth)
		return -EINVAL;
	if (set->queue_depth < set->reserved_tags + BLK_MQ_TAG_MIN)
		return -EINVAL;

	if (!set->ops->queue_rq || !set->ops->map_queue)
		return -EINVAL;

	if (set->queue_depth > BLK_MQ_MAX_DEPTH) {
		pr_info("blk-mq: reduced tag depth to %u\n",
			BLK_MQ_MAX_DEPTH);
		set->queue_depth = BLK_MQ_MAX_DEPTH;
	}

	/*
	 * If a crashdump is active, then we are potentially in a very
	 * memory constrained environment. Limit us to 1 queue and
	 * 64 tags to prevent using too much memory.
	 */
	if (is_kdump_kernel()) {
		set->nr_hw_queues = 1;
		set->queue_depth = min(64U, set->queue_depth);
	}

	set->tags = kmalloc_node(set->nr_hw_queues *
				 sizeof(struct blk_mq_tags *),
				 GFP_KERNEL, set->numa_node);
	if (!set->tags)
		return -ENOMEM;

	if (blk_mq_alloc_rq_maps(set))
		goto enomem;

	mutex_init(&set->tag_list_lock);
	INIT_LIST_HEAD(&set->tag_list);

	return 0;
enomem:
	kfree(set->tags);
	set->tags = NULL;
	return -ENOMEM;
}
