void blk_delay_queue(struct request_queue *q, unsigned long msecs)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	if (likely(!blk_queue_dead(q)))
		queue_delayed_work(kblockd_workqueue, &q->delay_work,
				   msecs_to_jiffies(msecs));
}
