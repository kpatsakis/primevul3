struct blk_flush_queue *blk_alloc_flush_queue(struct request_queue *q,
		int node, int cmd_size)
{
	struct blk_flush_queue *fq;
	int rq_sz = sizeof(struct request);

	fq = kzalloc_node(sizeof(*fq), GFP_KERNEL, node);
	if (!fq)
		goto fail;

	if (q->mq_ops) {
		spin_lock_init(&fq->mq_flush_lock);
		rq_sz = round_up(rq_sz + cmd_size, cache_line_size());
	}

	fq->flush_rq = kzalloc_node(rq_sz, GFP_KERNEL, node);
	if (!fq->flush_rq)
		goto fail_rq;

	INIT_LIST_HEAD(&fq->flush_queue[0]);
	INIT_LIST_HEAD(&fq->flush_queue[1]);
	INIT_LIST_HEAD(&fq->flush_data_in_flight);

	return fq;

 fail_rq:
	kfree(fq);
 fail:
	return NULL;
}
