megasas_wait_for_adapter_operational(struct megasas_instance *instance)
{
	int wait_time = MEGASAS_RESET_WAIT_TIME * 2;
	int i;
	u8 adp_state;

	for (i = 0; i < wait_time; i++) {
		adp_state = atomic_read(&instance->adprecovery);
		if ((adp_state == MEGASAS_HBA_OPERATIONAL) ||
		    (adp_state == MEGASAS_HW_CRITICAL_ERROR))
			break;

		if (!(i % MEGASAS_RESET_NOTICE_INTERVAL))
			dev_notice(&instance->pdev->dev, "waiting for controller reset to finish\n");

		msleep(1000);
	}

	if (adp_state != MEGASAS_HBA_OPERATIONAL) {
		dev_info(&instance->pdev->dev,
			 "%s HBA failed to become operational, adp_state %d\n",
			 __func__, adp_state);
		return 1;
	}

	return 0;
}
