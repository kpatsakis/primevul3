inline void __blk_run_queue_uncond(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	if (unlikely(blk_queue_dead(q)))
		return;

	/*
	 * Some request_fn implementations, e.g. scsi_request_fn(), unlock
	 * the queue lock internally. As a result multiple threads may be
	 * running such a request function concurrently. Keep track of the
	 * number of active request_fn invocations such that blk_drain_queue()
	 * can wait until all these request_fn calls have finished.
	 */
	q->request_fn_active++;
	q->request_fn(q);
	q->request_fn_active--;
}
