megasas_issue_blocked_abort_cmd(struct megasas_instance *instance,
				struct megasas_cmd *cmd_to_abort, int timeout)
{
	struct megasas_cmd *cmd;
	struct megasas_abort_frame *abort_fr;
	int ret = 0;

	cmd = megasas_get_cmd(instance);

	if (!cmd)
		return -1;

	abort_fr = &cmd->frame->abort;

	/*
	 * Prepare and issue the abort frame
	 */
	abort_fr->cmd = MFI_CMD_ABORT;
	abort_fr->cmd_status = MFI_STAT_INVALID_STATUS;
	abort_fr->flags = cpu_to_le16(0);
	abort_fr->abort_context = cpu_to_le32(cmd_to_abort->index);
	abort_fr->abort_mfi_phys_addr_lo =
		cpu_to_le32(lower_32_bits(cmd_to_abort->frame_phys_addr));
	abort_fr->abort_mfi_phys_addr_hi =
		cpu_to_le32(upper_32_bits(cmd_to_abort->frame_phys_addr));

	cmd->sync_cmd = 1;
	cmd->cmd_status_drv = MFI_STAT_INVALID_STATUS;

	if (atomic_read(&instance->adprecovery) == MEGASAS_HW_CRITICAL_ERROR) {
		dev_err(&instance->pdev->dev, "Failed from %s %d\n",
			__func__, __LINE__);
		return DCMD_NOT_FIRED;
	}

	instance->instancet->issue_dcmd(instance, cmd);

	if (timeout) {
		ret = wait_event_timeout(instance->abort_cmd_wait_q,
				cmd->cmd_status_drv != MFI_STAT_INVALID_STATUS, timeout * HZ);
		if (!ret) {
			dev_err(&instance->pdev->dev, "Failed from %s %d Abort Timed out\n",
				__func__, __LINE__);
			return DCMD_TIMEOUT;
		}
	} else
		wait_event(instance->abort_cmd_wait_q,
				cmd->cmd_status_drv != MFI_STAT_INVALID_STATUS);

	cmd->sync_cmd = 0;

	megasas_return_cmd(instance, cmd);
	return (cmd->cmd_status_drv == MFI_STAT_OK) ?
		DCMD_SUCCESS : DCMD_FAILED;
}
