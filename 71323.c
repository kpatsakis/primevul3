megasas_issue_polled(struct megasas_instance *instance, struct megasas_cmd *cmd)
{
	struct megasas_header *frame_hdr = &cmd->frame->hdr;

	frame_hdr->cmd_status = MFI_STAT_INVALID_STATUS;
	frame_hdr->flags |= cpu_to_le16(MFI_FRAME_DONT_POST_IN_REPLY_QUEUE);

	if (atomic_read(&instance->adprecovery) == MEGASAS_HW_CRITICAL_ERROR) {
		dev_err(&instance->pdev->dev, "Failed from %s %d\n",
			__func__, __LINE__);
		return DCMD_NOT_FIRED;
	}

	instance->instancet->issue_dcmd(instance, cmd);

	return wait_and_poll(instance, cmd, instance->requestorId ?
			MEGASAS_ROUTINE_WAIT_TIME_VF : MFI_IO_TIMEOUT_SECS);
}
