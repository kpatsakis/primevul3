static int blk_mq_alloc_rq_maps(struct blk_mq_tag_set *set)
{
	unsigned int depth;
	int err;

	depth = set->queue_depth;
	do {
		err = __blk_mq_alloc_rq_maps(set);
		if (!err)
			break;

		set->queue_depth >>= 1;
		if (set->queue_depth < set->reserved_tags + BLK_MQ_TAG_MIN) {
			err = -ENOMEM;
			break;
		}
	} while (set->queue_depth);

	if (!set->queue_depth || err) {
		pr_err("blk-mq: failed to allocate request map\n");
		return -ENOMEM;
	}

	if (depth != set->queue_depth)
		pr_info("blk-mq: reduced tag depth (%u -> %u)\n",
						depth, set->queue_depth);

	return 0;
}
