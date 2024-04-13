update_stats_enqueue(struct cfs_rq *cfs_rq, struct sched_entity *se, int flags)
{
	if (!schedstat_enabled())
		return;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	if (se != cfs_rq->curr)
		update_stats_wait_start(cfs_rq, se);

	if (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper(cfs_rq, se);
}
