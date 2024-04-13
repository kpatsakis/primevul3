static void *alloc_request_size(gfp_t gfp_mask, void *data)
{
	struct request_queue *q = data;
	struct request *rq;

	rq = kmalloc_node(sizeof(struct request) + q->cmd_size, gfp_mask,
			q->node);
	if (rq && q->init_rq_fn && q->init_rq_fn(q, rq, gfp_mask) < 0) {
		kfree(rq);
		rq = NULL;
	}
	return rq;
}
