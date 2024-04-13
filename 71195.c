static int ipmi_thread(void *data)
{
	struct smi_info *smi_info = data;
	unsigned long flags;
	enum si_sm_result smi_result;
	struct timespec64 busy_until;

	ipmi_si_set_not_busy(&busy_until);
	set_user_nice(current, MAX_NICE);
	while (!kthread_should_stop()) {
		int busy_wait;

		spin_lock_irqsave(&(smi_info->si_lock), flags);
		smi_result = smi_event_handler(smi_info, 0);

		/*
		 * If the driver is doing something, there is a possible
		 * race with the timer.  If the timer handler see idle,
		 * and the thread here sees something else, the timer
		 * handler won't restart the timer even though it is
		 * required.  So start it here if necessary.
		 */
		if (smi_result != SI_SM_IDLE && !smi_info->timer_running)
			smi_mod_timer(smi_info, jiffies + SI_TIMEOUT_JIFFIES);

		spin_unlock_irqrestore(&(smi_info->si_lock), flags);
		busy_wait = ipmi_thread_busy_wait(smi_result, smi_info,
						  &busy_until);
		if (smi_result == SI_SM_CALL_WITHOUT_DELAY)
			; /* do nothing */
		else if (smi_result == SI_SM_CALL_WITH_DELAY && busy_wait)
			schedule();
		else if (smi_result == SI_SM_IDLE) {
			if (atomic_read(&smi_info->need_watch)) {
				schedule_timeout_interruptible(100);
			} else {
				/* Wait to be woken up when we are needed. */
				__set_current_state(TASK_INTERRUPTIBLE);
				schedule();
			}
		} else
			schedule_timeout_interruptible(1);
	}
	return 0;
}
