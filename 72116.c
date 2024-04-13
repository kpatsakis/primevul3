void blk_queue_flag_clear(unsigned int flag, struct request_queue *q)
{
	unsigned long flags;

	spin_lock_irqsave(q->queue_lock, flags);
	queue_flag_clear(flag, q);
	spin_unlock_irqrestore(q->queue_lock, flags);
}
