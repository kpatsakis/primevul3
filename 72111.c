void blk_queue_bypass_end(struct request_queue *q)
{
	spin_lock_irq(q->queue_lock);
	if (!--q->bypass_depth)
		queue_flag_clear(QUEUE_FLAG_BYPASS, q);
	WARN_ON_ONCE(q->bypass_depth < 0);
	spin_unlock_irq(q->queue_lock);
}
