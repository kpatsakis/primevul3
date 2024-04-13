void blk_run_queue(struct request_queue *q)
{
	unsigned long flags;

	WARN_ON_ONCE(q->mq_ops);

	spin_lock_irqsave(q->queue_lock, flags);
	__blk_run_queue(q);
	spin_unlock_irqrestore(q->queue_lock, flags);
}
