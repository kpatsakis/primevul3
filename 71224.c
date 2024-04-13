static inline void stop_timer_and_thread(struct smi_info *smi_info)
{
	if (smi_info->thread != NULL) {
		kthread_stop(smi_info->thread);
		smi_info->thread = NULL;
	}

	smi_info->timer_can_start = false;
	if (smi_info->timer_running)
		del_timer_sync(&smi_info->si_timer);
}
