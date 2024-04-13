void blk_post_runtime_suspend(struct request_queue *q, int err)
{
	if (!q->dev)
		return;

	spin_lock_irq(q->queue_lock);
	if (!err) {
		q->rpm_status = RPM_SUSPENDED;
	} else {
		q->rpm_status = RPM_ACTIVE;
		pm_runtime_mark_last_busy(q->dev);
	}
	spin_unlock_irq(q->queue_lock);
}
