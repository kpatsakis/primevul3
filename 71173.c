static void check_start_timer_thread(struct smi_info *smi_info)
{
	if (smi_info->si_state == SI_NORMAL && smi_info->curr_msg == NULL) {
		smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);

		if (smi_info->thread)
			wake_up_process(smi_info->thread);

		start_next_msg(smi_info);
		smi_event_handler(smi_info, 0);
	}
}
