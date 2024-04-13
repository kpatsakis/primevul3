void blk_mq_free_tag_set(struct blk_mq_tag_set *set)
{
	int i;

	for (i = 0; i < set->nr_hw_queues; i++) {
		if (set->tags[i]) {
			blk_mq_free_rq_map(set, set->tags[i], i);
			free_cpumask_var(set->tags[i]->cpumask);
		}
	}

	kfree(set->tags);
	set->tags = NULL;
}
