static void poll(void *send_info)
{
	struct smi_info *smi_info = send_info;
	unsigned long flags = 0;
	bool run_to_completion = smi_info->run_to_completion;

	/*
	 * Make sure there is some delay in the poll loop so we can
	 * drive time forward and timeout things.
	 */
	udelay(10);
	if (!run_to_completion)
		spin_lock_irqsave(&smi_info->si_lock, flags);
	smi_event_handler(smi_info, 10);
	if (!run_to_completion)
		spin_unlock_irqrestore(&smi_info->si_lock, flags);
}
