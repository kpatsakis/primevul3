static void blk_delay_work(struct work_struct *work)
{
	struct request_queue *q;

	q = container_of(work, struct request_queue, delay_work.work);
	spin_lock_irq(q->queue_lock);
	__blk_run_queue(q);
	spin_unlock_irq(q->queue_lock);
}
