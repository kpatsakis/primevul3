blk_init_queue_node(request_fn_proc *rfn, spinlock_t *lock, int node_id)
{
	struct request_queue *q;

	q = blk_alloc_queue_node(GFP_KERNEL, node_id, lock);
	if (!q)
		return NULL;

	q->request_fn = rfn;
	if (blk_init_allocated_queue(q) < 0) {
		blk_cleanup_queue(q);
		return NULL;
	}

	return q;
}
