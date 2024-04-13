bool blk_queue_flag_test_and_clear(unsigned int flag, struct request_queue *q)
{
	unsigned long flags;
	bool res;

	spin_lock_irqsave(q->queue_lock, flags);
	res = queue_flag_test_and_clear(flag, q);
	spin_unlock_irqrestore(q->queue_lock, flags);

	return res;
}
