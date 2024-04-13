static void smi_mod_timer(struct smi_info *smi_info, unsigned long new_val)
{
	if (!smi_info->timer_can_start)
		return;
	smi_info->last_timeout_jiffies = jiffies;
	mod_timer(&smi_info->si_timer, new_val);
	smi_info->timer_running = true;
}
