void blk_pre_runtime_resume(struct request_queue *q)
{
	if (!q->dev)
		return;

	spin_lock_irq(q->queue_lock);
	q->rpm_status = RPM_RESUMING;
	spin_unlock_irq(q->queue_lock);
}
