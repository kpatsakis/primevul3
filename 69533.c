static int __blk_mq_alloc_rq_maps(struct blk_mq_tag_set *set)
{
	int i;

	for (i = 0; i < set->nr_hw_queues; i++) {
		set->tags[i] = blk_mq_init_rq_map(set, i);
		if (!set->tags[i])
			goto out_unwind;
	}

	return 0;

out_unwind:
	while (--i >= 0)
		blk_mq_free_rq_map(set, set->tags[i], i);

	return -ENOMEM;
}
