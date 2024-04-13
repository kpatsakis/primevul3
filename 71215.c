static int smi_start_processing(void            *send_info,
				struct ipmi_smi *intf)
{
	struct smi_info *new_smi = send_info;
	int             enable = 0;

	new_smi->intf = intf;

	/* Set up the timer that drives the interface. */
	timer_setup(&new_smi->si_timer, smi_timeout, 0);
	new_smi->timer_can_start = true;
	smi_mod_timer(new_smi, jiffies + SI_TIMEOUT_JIFFIES);

	/* Try to claim any interrupts. */
	if (new_smi->io.irq_setup) {
		new_smi->io.irq_handler_data = new_smi;
		new_smi->io.irq_setup(&new_smi->io);
	}

	/*
	 * Check if the user forcefully enabled the daemon.
	 */
	if (new_smi->si_num < num_force_kipmid)
		enable = force_kipmid[new_smi->si_num];
	/*
	 * The BT interface is efficient enough to not need a thread,
	 * and there is no need for a thread if we have interrupts.
	 */
	else if ((new_smi->io.si_type != SI_BT) && (!new_smi->io.irq))
		enable = 1;

	if (enable) {
		new_smi->thread = kthread_run(ipmi_thread, new_smi,
					      "kipmi%d", new_smi->si_num);
		if (IS_ERR(new_smi->thread)) {
			dev_notice(new_smi->io.dev, "Could not start"
				   " kernel thread due to error %ld, only using"
				   " timers to drive the interface\n",
				   PTR_ERR(new_smi->thread));
			new_smi->thread = NULL;
		}
	}

	return 0;
}
