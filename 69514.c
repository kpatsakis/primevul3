struct blk_mq_tags *blk_mq_init_tags(unsigned int total_tags,
				     unsigned int reserved_tags,
				     int node, int alloc_policy)
{
	struct blk_mq_tags *tags;

	if (total_tags > BLK_MQ_TAG_MAX) {
		pr_err("blk-mq: tag depth too large\n");
		return NULL;
	}

	tags = kzalloc_node(sizeof(*tags), GFP_KERNEL, node);
	if (!tags)
		return NULL;

	if (!zalloc_cpumask_var(&tags->cpumask, GFP_KERNEL)) {
		kfree(tags);
		return NULL;
	}

	tags->nr_tags = total_tags;
	tags->nr_reserved_tags = reserved_tags;

	return blk_mq_init_bitmap_tags(tags, node, alloc_policy);
}
