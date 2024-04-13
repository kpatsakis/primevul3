megasas_get_target_prop(struct megasas_instance *instance,
			struct scsi_device *sdev)
{
	int ret;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	u16 targetId = (sdev->channel % 2) + sdev->id;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_err(&instance->pdev->dev,
			"Failed to get cmd %s\n", __func__);
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;

	memset(instance->tgt_prop, 0, sizeof(*instance->tgt_prop));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);
	dcmd->mbox.b[0] = MEGASAS_IS_LOGICAL(sdev);

	dcmd->mbox.s[1] = cpu_to_le16(targetId);
	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = 0xFF;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len =
		cpu_to_le32(sizeof(struct MR_TARGET_PROPERTIES));
	dcmd->opcode = cpu_to_le32(MR_DCMD_DRV_GET_TARGET_PROP);

	megasas_set_dma_settings(instance, dcmd, instance->tgt_prop_h,
				 sizeof(struct MR_TARGET_PROPERTIES));

	if ((instance->adapter_type != MFI_SERIES) &&
	    !instance->mask_interrupts)
		ret = megasas_issue_blocked_cmd(instance,
						cmd, MFI_IO_TIMEOUT_SECS);
	else
		ret = megasas_issue_polled(instance, cmd);

	switch (ret) {
	case DCMD_TIMEOUT:
		switch (dcmd_timeout_ocr_possible(instance)) {
		case INITIATE_OCR:
			cmd->flags |= DRV_DCMD_SKIP_REFIRE;
			megasas_reset_fusion(instance->host,
					     MFI_IO_TIMEOUT_OCR);
			break;
		case KILL_ADAPTER:
			megaraid_sas_kill_hba(instance);
			break;
		case IGNORE_TIMEOUT:
			dev_info(&instance->pdev->dev,
				 "Ignore DCMD timeout: %s %d\n",
				 __func__, __LINE__);
			break;
		}
		break;

	default:
		megasas_return_cmd(instance, cmd);
	}
	if (ret != DCMD_SUCCESS)
		dev_err(&instance->pdev->dev,
			"return from %s %d return value %d\n",
			__func__, __LINE__, ret);

	return ret;
}
