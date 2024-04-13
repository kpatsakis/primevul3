void blk_set_runtime_active(struct request_queue *q)
{
	spin_lock_irq(q->queue_lock);
	q->rpm_status = RPM_ACTIVE;
	pm_runtime_mark_last_busy(q->dev);
	pm_request_autosuspend(q->dev);
	spin_unlock_irq(q->queue_lock);
}
