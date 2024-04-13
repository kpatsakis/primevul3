megasas_issue_blocked_cmd(struct megasas_instance *instance,
			  struct megasas_cmd *cmd, int timeout)
{
	int ret = 0;
	cmd->cmd_status_drv = MFI_STAT_INVALID_STATUS;

	if (atomic_read(&instance->adprecovery) == MEGASAS_HW_CRITICAL_ERROR) {
		dev_err(&instance->pdev->dev, "Failed from %s %d\n",
			__func__, __LINE__);
		return DCMD_NOT_FIRED;
	}

	instance->instancet->issue_dcmd(instance, cmd);

	if (timeout) {
		ret = wait_event_timeout(instance->int_cmd_wait_q,
				cmd->cmd_status_drv != MFI_STAT_INVALID_STATUS, timeout * HZ);
		if (!ret) {
			dev_err(&instance->pdev->dev, "Failed from %s %d DCMD Timed out\n",
				__func__, __LINE__);
			return DCMD_TIMEOUT;
		}
	} else
		wait_event(instance->int_cmd_wait_q,
				cmd->cmd_status_drv != MFI_STAT_INVALID_STATUS);

	return (cmd->cmd_status_drv == MFI_STAT_OK) ?
		DCMD_SUCCESS : DCMD_FAILED;
}
