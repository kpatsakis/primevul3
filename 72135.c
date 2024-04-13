void blk_start_queue_async(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	queue_flag_clear(QUEUE_FLAG_STOPPED, q);
	blk_run_queue_async(q);
}
