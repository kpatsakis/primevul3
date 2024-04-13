megasas_get_pd_list(struct megasas_instance *instance)
{
	int ret = 0, pd_index = 0;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	struct MR_PD_LIST *ci;
	struct MR_PD_ADDRESS *pd_addr;
	dma_addr_t ci_h = 0;

	if (instance->pd_list_not_supported) {
		dev_info(&instance->pdev->dev, "MR_DCMD_PD_LIST_QUERY "
		"not supported by firmware\n");
		return ret;
	}

	ci = instance->pd_list_buf;
	ci_h = instance->pd_list_buf_h;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_printk(KERN_DEBUG, &instance->pdev->dev, "(get_pd_list): Failed to get cmd\n");
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;

	memset(ci, 0, sizeof(*ci));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->mbox.b[0] = MR_PD_QUERY_TYPE_EXPOSED_TO_HOST;
	dcmd->mbox.b[1] = 0;
	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = MFI_STAT_INVALID_STATUS;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len = cpu_to_le32(MEGASAS_MAX_PD * sizeof(struct MR_PD_LIST));
	dcmd->opcode = cpu_to_le32(MR_DCMD_PD_LIST_QUERY);

	megasas_set_dma_settings(instance, dcmd, instance->pd_list_buf_h,
				 (MEGASAS_MAX_PD * sizeof(struct MR_PD_LIST)));

	if ((instance->adapter_type != MFI_SERIES) &&
	    !instance->mask_interrupts)
		ret = megasas_issue_blocked_cmd(instance, cmd,
			MFI_IO_TIMEOUT_SECS);
	else
		ret = megasas_issue_polled(instance, cmd);

	switch (ret) {
	case DCMD_FAILED:
		dev_info(&instance->pdev->dev, "MR_DCMD_PD_LIST_QUERY "
			"failed/not supported by firmware\n");

		if (instance->adapter_type != MFI_SERIES)
			megaraid_sas_kill_hba(instance);
		else
			instance->pd_list_not_supported = 1;
		break;
	case DCMD_TIMEOUT:

		switch (dcmd_timeout_ocr_possible(instance)) {
		case INITIATE_OCR:
			cmd->flags |= DRV_DCMD_SKIP_REFIRE;
			/*
			 * DCMD failed from AEN path.
			 * AEN path already hold reset_mutex to avoid PCI access
			 * while OCR is in progress.
			 */
			mutex_unlock(&instance->reset_mutex);
			megasas_reset_fusion(instance->host,
						MFI_IO_TIMEOUT_OCR);
			mutex_lock(&instance->reset_mutex);
			break;
		case KILL_ADAPTER:
			megaraid_sas_kill_hba(instance);
			break;
		case IGNORE_TIMEOUT:
			dev_info(&instance->pdev->dev, "Ignore DCMD timeout: %s %d \n",
				__func__, __LINE__);
			break;
		}

		break;

	case DCMD_SUCCESS:
		pd_addr = ci->addr;

		if ((le32_to_cpu(ci->count) >
			(MEGASAS_MAX_PD_CHANNELS * MEGASAS_MAX_DEV_PER_CHANNEL)))
			break;

		memset(instance->local_pd_list, 0,
				MEGASAS_MAX_PD * sizeof(struct megasas_pd_list));

		for (pd_index = 0; pd_index < le32_to_cpu(ci->count); pd_index++) {
			instance->local_pd_list[le16_to_cpu(pd_addr->deviceId)].tid	=
					le16_to_cpu(pd_addr->deviceId);
			instance->local_pd_list[le16_to_cpu(pd_addr->deviceId)].driveType	=
					pd_addr->scsiDevType;
			instance->local_pd_list[le16_to_cpu(pd_addr->deviceId)].driveState	=
					MR_PD_STATE_SYSTEM;
			pd_addr++;
		}

		memcpy(instance->pd_list, instance->local_pd_list,
			sizeof(instance->pd_list));
		break;

	}

	if (ret != DCMD_TIMEOUT)
		megasas_return_cmd(instance, cmd);

	return ret;
}
