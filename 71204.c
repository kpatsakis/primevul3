static void sender(void                *send_info,
		   struct ipmi_smi_msg *msg)
{
	struct smi_info   *smi_info = send_info;
	unsigned long     flags;

	debug_timestamp("Enqueue");

	if (smi_info->run_to_completion) {
		/*
		 * If we are running to completion, start it.  Upper
		 * layer will call flush_messages to clear it out.
		 */
		smi_info->waiting_msg = msg;
		return;
	}

	spin_lock_irqsave(&smi_info->si_lock, flags);
	/*
	 * The following two lines don't need to be under the lock for
	 * the lock's sake, but they do need SMP memory barriers to
	 * avoid getting things out of order.  We are already claiming
	 * the lock, anyway, so just do it under the lock to avoid the
	 * ordering problem.
	 */
	BUG_ON(smi_info->waiting_msg);
	smi_info->waiting_msg = msg;
	check_start_timer_thread(smi_info);
	spin_unlock_irqrestore(&smi_info->si_lock, flags);
}
