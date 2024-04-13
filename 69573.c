static void blk_mq_init_cpu_queues(struct request_queue *q,
				   unsigned int nr_hw_queues)
{
	unsigned int i;

	for_each_possible_cpu(i) {
		struct blk_mq_ctx *__ctx = per_cpu_ptr(q->queue_ctx, i);
		struct blk_mq_hw_ctx *hctx;

		memset(__ctx, 0, sizeof(*__ctx));
		__ctx->cpu = i;
		spin_lock_init(&__ctx->lock);
		INIT_LIST_HEAD(&__ctx->rq_list);
		__ctx->queue = q;

		/* If the cpu isn't online, the cpu is mapped to first hctx */
		if (!cpu_online(i))
			continue;

		hctx = q->mq_ops->map_queue(q, i);

		/*
		 * Set local node, IFF we have more than one hw queue. If
		 * not, we remain on the home node of the device
		 */
		if (nr_hw_queues > 1 && hctx->numa_node == NUMA_NO_NODE)
			hctx->numa_node = cpu_to_node(i);
	}
}
