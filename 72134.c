void blk_start_queue(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	queue_flag_clear(QUEUE_FLAG_STOPPED, q);
	__blk_run_queue(q);
}
