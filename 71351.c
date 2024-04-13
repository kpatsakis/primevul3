megasas_service_aen(struct megasas_instance *instance, struct megasas_cmd *cmd)
{
	unsigned long flags;

	/*
	 * Don't signal app if it is just an aborted previously registered aen
	 */
	if ((!cmd->abort_aen) && (instance->unload == 0)) {
		spin_lock_irqsave(&poll_aen_lock, flags);
		megasas_poll_wait_aen = 1;
		spin_unlock_irqrestore(&poll_aen_lock, flags);
		wake_up(&megasas_poll_wait);
		kill_fasync(&megasas_async_queue, SIGIO, POLL_IN);
	}
	else
		cmd->abort_aen = 0;

	instance->aen_cmd = NULL;

	megasas_return_cmd(instance, cmd);

	if ((instance->unload == 0) &&
		((instance->issuepend_done == 1))) {
		struct megasas_aen_event *ev;

		ev = kzalloc(sizeof(*ev), GFP_ATOMIC);
		if (!ev) {
			dev_err(&instance->pdev->dev, "megasas_service_aen: out of memory\n");
		} else {
			ev->instance = instance;
			instance->ev = ev;
			INIT_DELAYED_WORK(&ev->hotplug_work,
					  megasas_aen_polling);
			schedule_delayed_work(&ev->hotplug_work, 0);
		}
	}
}
