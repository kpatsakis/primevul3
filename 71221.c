static void start_new_msg(struct smi_info *smi_info, unsigned char *msg,
			  unsigned int size)
{
	smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);

	if (smi_info->thread)
		wake_up_process(smi_info->thread);

	smi_info->handlers->start_transaction(smi_info->si_sm, msg, size);
}
