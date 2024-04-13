megasas_get_pd_info(struct megasas_instance *instance, struct scsi_device *sdev)
{
	int ret;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;

	struct MR_PRIV_DEVICE *mr_device_priv_data;
	u16 device_id = 0;

	device_id = (sdev->channel * MEGASAS_MAX_DEV_PER_CHANNEL) + sdev->id;
	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_err(&instance->pdev->dev, "Failed to get cmd %s\n", __func__);
		return;
	}

	dcmd = &cmd->frame->dcmd;

	memset(instance->pd_info, 0, sizeof(*instance->pd_info));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->mbox.s[0] = cpu_to_le16(device_id);
	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = 0xFF;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len = cpu_to_le32(sizeof(struct MR_PD_INFO));
	dcmd->opcode = cpu_to_le32(MR_DCMD_PD_GET_INFO);

	megasas_set_dma_settings(instance, dcmd, instance->pd_info_h,
				 sizeof(struct MR_PD_INFO));

	if ((instance->adapter_type != MFI_SERIES) &&
	    !instance->mask_interrupts)
		ret = megasas_issue_blocked_cmd(instance, cmd, MFI_IO_TIMEOUT_SECS);
	else
		ret = megasas_issue_polled(instance, cmd);

	switch (ret) {
	case DCMD_SUCCESS:
		mr_device_priv_data = sdev->hostdata;
		le16_to_cpus((u16 *)&instance->pd_info->state.ddf.pdType);
		mr_device_priv_data->interface_type =
				instance->pd_info->state.ddf.pdType.intf;
		break;

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
			dev_info(&instance->pdev->dev, "Ignore DCMD timeout: %s %d\n",
				__func__, __LINE__);
			break;
		}

		break;
	}

	if (ret != DCMD_TIMEOUT)
		megasas_return_cmd(instance, cmd);

	return;
}
