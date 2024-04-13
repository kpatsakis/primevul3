static void set_need_watch(void *send_info, bool enable)
{
	struct smi_info *smi_info = send_info;
	unsigned long flags;

	atomic_set(&smi_info->need_watch, enable);
	spin_lock_irqsave(&smi_info->si_lock, flags);
	check_start_timer_thread(smi_info);
	spin_unlock_irqrestore(&smi_info->si_lock, flags);
}
