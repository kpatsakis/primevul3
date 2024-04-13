void blk_exit_rl(struct request_queue *q, struct request_list *rl)
{
	if (rl->rq_pool) {
		mempool_destroy(rl->rq_pool);
		if (rl != &q->root_rl)
			blk_put_queue(q);
	}
}
