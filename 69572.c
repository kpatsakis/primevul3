struct request_queue *blk_mq_init_allocated_queue(struct blk_mq_tag_set *set,
						  struct request_queue *q)
{
	struct blk_mq_hw_ctx **hctxs;
	struct blk_mq_ctx __percpu *ctx;
	unsigned int *map;
	int i;

	ctx = alloc_percpu(struct blk_mq_ctx);
	if (!ctx)
		return ERR_PTR(-ENOMEM);

	hctxs = kmalloc_node(set->nr_hw_queues * sizeof(*hctxs), GFP_KERNEL,
			set->numa_node);

	if (!hctxs)
		goto err_percpu;

	map = blk_mq_make_queue_map(set);
	if (!map)
		goto err_map;

	for (i = 0; i < set->nr_hw_queues; i++) {
		int node = blk_mq_hw_queue_to_node(map, i);

		hctxs[i] = kzalloc_node(sizeof(struct blk_mq_hw_ctx),
					GFP_KERNEL, node);
		if (!hctxs[i])
			goto err_hctxs;

		if (!zalloc_cpumask_var_node(&hctxs[i]->cpumask, GFP_KERNEL,
						node))
			goto err_hctxs;

		atomic_set(&hctxs[i]->nr_active, 0);
		hctxs[i]->numa_node = node;
		hctxs[i]->queue_num = i;
	}

	/*
	 * Init percpu_ref in atomic mode so that it's faster to shutdown.
	 * See blk_register_queue() for details.
	 */
	if (percpu_ref_init(&q->mq_usage_counter, blk_mq_usage_counter_release,
			    PERCPU_REF_INIT_ATOMIC, GFP_KERNEL))
		goto err_hctxs;

	setup_timer(&q->timeout, blk_mq_rq_timer, (unsigned long) q);
	blk_queue_rq_timeout(q, set->timeout ? set->timeout : 30 * HZ);

	q->nr_queues = nr_cpu_ids;
	q->nr_hw_queues = set->nr_hw_queues;
	q->mq_map = map;

	q->queue_ctx = ctx;
	q->queue_hw_ctx = hctxs;

	q->mq_ops = set->ops;
	q->queue_flags |= QUEUE_FLAG_MQ_DEFAULT;

	if (!(set->flags & BLK_MQ_F_SG_MERGE))
		q->queue_flags |= 1 << QUEUE_FLAG_NO_SG_MERGE;

	q->sg_reserved_size = INT_MAX;

	INIT_WORK(&q->requeue_work, blk_mq_requeue_work);
	INIT_LIST_HEAD(&q->requeue_list);
	spin_lock_init(&q->requeue_lock);

	if (q->nr_hw_queues > 1)
		blk_queue_make_request(q, blk_mq_make_request);
	else
		blk_queue_make_request(q, blk_sq_make_request);

	/*
	 * Do this after blk_queue_make_request() overrides it...
	 */
	q->nr_requests = set->queue_depth;

	if (set->ops->complete)
		blk_queue_softirq_done(q, set->ops->complete);

	blk_mq_init_cpu_queues(q, set->nr_hw_queues);

	if (blk_mq_init_hw_queues(q, set))
		goto err_hctxs;

	mutex_lock(&all_q_mutex);
	list_add_tail(&q->all_q_node, &all_q_list);
	mutex_unlock(&all_q_mutex);

	blk_mq_add_queue_tag_set(set, q);

	blk_mq_map_swqueue(q);

	return q;

err_hctxs:
	kfree(map);
	for (i = 0; i < set->nr_hw_queues; i++) {
		if (!hctxs[i])
			break;
		free_cpumask_var(hctxs[i]->cpumask);
		kfree(hctxs[i]);
	}
err_map:
	kfree(hctxs);
err_percpu:
	free_percpu(ctx);
	return ERR_PTR(-ENOMEM);
}
