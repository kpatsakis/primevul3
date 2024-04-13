static inline u64 cfs_rq_clock_task(struct cfs_rq *cfs_rq)
{
	if (unlikely(cfs_rq->throttle_count))
		return cfs_rq->throttled_clock_task - cfs_rq->throttled_clock_task_time;

	return rq_clock_task(rq_of(cfs_rq)) - cfs_rq->throttled_clock_task_time;
}
