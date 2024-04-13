megasas_check_and_restore_queue_depth(struct megasas_instance *instance)
{
	unsigned long flags;

	if (instance->flag & MEGASAS_FW_BUSY
	    && time_after(jiffies, instance->last_time + 5 * HZ)
	    && atomic_read(&instance->fw_outstanding) <
	    instance->throttlequeuedepth + 1) {

		spin_lock_irqsave(instance->host->host_lock, flags);
		instance->flag &= ~MEGASAS_FW_BUSY;

		instance->host->can_queue = instance->cur_can_queue;
		spin_unlock_irqrestore(instance->host->host_lock, flags);
	}
}
