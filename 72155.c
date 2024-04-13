static inline int ioc_batching(struct request_queue *q, struct io_context *ioc)
{
	if (!ioc)
		return 0;

	/*
	 * Make sure the process is able to allocate at least 1 request
	 * even if the batch times out, otherwise we could theoretically
	 * lose wakeups.
	 */
	return ioc->nr_batch_requests == q->nr_batching ||
		(ioc->nr_batch_requests > 0
		&& time_before(jiffies, ioc->last_waited + BLK_BATCH_TIME));
}
