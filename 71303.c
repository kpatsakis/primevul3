megasas_get_ld_list(struct megasas_instance *instance)
{
	int ret = 0, ld_index = 0, ids = 0;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	struct MR_LD_LIST *ci;
	dma_addr_t ci_h = 0;
	u32 ld_count;

	ci = instance->ld_list_buf;
	ci_h = instance->ld_list_buf_h;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_printk(KERN_DEBUG, &instance->pdev->dev, "megasas_get_ld_list: Failed to get cmd\n");
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;

	memset(ci, 0, sizeof(*ci));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	if (instance->supportmax256vd)
		dcmd->mbox.b[0] = 1;
	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = MFI_STAT_INVALID_STATUS;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->data_xfer_len = cpu_to_le32(sizeof(struct MR_LD_LIST));
	dcmd->opcode = cpu_to_le32(MR_DCMD_LD_GET_LIST);
	dcmd->pad_0  = 0;

	megasas_set_dma_settings(instance, dcmd, ci_h,
				 sizeof(struct MR_LD_LIST));

	if ((instance->adapter_type != MFI_SERIES) &&
	    !instance->mask_interrupts)
		ret = megasas_issue_blocked_cmd(instance, cmd,
			MFI_IO_TIMEOUT_SECS);
	else
		ret = megasas_issue_polled(instance, cmd);

	ld_count = le32_to_cpu(ci->ldCount);

	switch (ret) {
	case DCMD_FAILED:
		megaraid_sas_kill_hba(instance);
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
			dev_info(&instance->pdev->dev, "Ignore DCMD timeout: %s %d\n",
				__func__, __LINE__);
			break;
		}

		break;

	case DCMD_SUCCESS:
		if (ld_count > instance->fw_supported_vd_count)
			break;

		memset(instance->ld_ids, 0xff, MAX_LOGICAL_DRIVES_EXT);

		for (ld_index = 0; ld_index < ld_count; ld_index++) {
			if (ci->ldList[ld_index].state != 0) {
				ids = ci->ldList[ld_index].ref.targetId;
				instance->ld_ids[ids] = ci->ldList[ld_index].ref.targetId;
			}
		}

		break;
	}

	if (ret != DCMD_TIMEOUT)
		megasas_return_cmd(instance, cmd);

	return ret;
}
