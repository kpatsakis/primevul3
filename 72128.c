void blk_run_queue_async(struct request_queue *q)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	if (likely(!blk_queue_stopped(q) && !blk_queue_dead(q)))
		mod_delayed_work(kblockd_workqueue, &q->delay_work, 0);
}
