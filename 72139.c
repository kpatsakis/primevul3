void blk_stop_queue(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	cancel_delayed_work(&q->delay_work);
	queue_flag_set(QUEUE_FLAG_STOPPED, q);
}
