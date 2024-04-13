void blk_queue_bypass_start(struct request_queue *q)
{
	WARN_ON_ONCE(q->mq_ops);

	spin_lock_irq(q->queue_lock);
	q->bypass_depth++;
	queue_flag_set(QUEUE_FLAG_BYPASS, q);
	spin_unlock_irq(q->queue_lock);

	/*
	 * Queues start drained.  Skip actual draining till init is
	 * complete.  This avoids lenghty delays during queue init which
	 * can happen many times during boot.
	 */
	if (blk_queue_init_done(q)) {
		spin_lock_irq(q->queue_lock);
		__blk_drain_queue(q, false);
		spin_unlock_irq(q->queue_lock);

		/* ensure blk_queue_bypass() is %true inside RCU read lock */
		synchronize_rcu();
	}
}
