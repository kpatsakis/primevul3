void blk_drain_queue(struct request_queue *q)
{
	spin_lock_irq(q->queue_lock);
	__blk_drain_queue(q, true);
	spin_unlock_irq(q->queue_lock);
}
