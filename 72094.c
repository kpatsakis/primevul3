int blk_init_rl(struct request_list *rl, struct request_queue *q,
		gfp_t gfp_mask)
{
	if (unlikely(rl->rq_pool) || q->mq_ops)
		return 0;

	rl->q = q;
	rl->count[BLK_RW_SYNC] = rl->count[BLK_RW_ASYNC] = 0;
	rl->starved[BLK_RW_SYNC] = rl->starved[BLK_RW_ASYNC] = 0;
	init_waitqueue_head(&rl->wait[BLK_RW_SYNC]);
	init_waitqueue_head(&rl->wait[BLK_RW_ASYNC]);

	if (q->cmd_size) {
		rl->rq_pool = mempool_create_node(BLKDEV_MIN_RQ,
				alloc_request_size, free_request_size,
				q, gfp_mask, q->node);
	} else {
		rl->rq_pool = mempool_create_node(BLKDEV_MIN_RQ,
				alloc_request_simple, free_request_simple,
				q, gfp_mask, q->node);
	}
	if (!rl->rq_pool)
		return -ENOMEM;

	if (rl != &q->root_rl)
		WARN_ON_ONCE(!blk_get_queue(q));

	return 0;
}
