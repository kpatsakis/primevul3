struct request_queue *blk_init_queue(request_fn_proc *rfn, spinlock_t *lock)
{
	return blk_init_queue_node(rfn, lock, NUMA_NO_NODE);
}
