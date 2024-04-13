void __blk_run_queue(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	if (unlikely(blk_queue_stopped(q)))
		return;

	__blk_run_queue_uncond(q);
}
