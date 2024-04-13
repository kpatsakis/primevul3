megasas_ld_list_query(struct megasas_instance *instance, u8 query_type)
{
	int ret = 0, ld_index = 0, ids = 0;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	struct MR_LD_TARGETID_LIST *ci;
	dma_addr_t ci_h = 0;
	u32 tgtid_count;

	ci = instance->ld_targetid_list_buf;
	ci_h = instance->ld_targetid_list_buf_h;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_warn(&instance->pdev->dev,
		         "megasas_ld_list_query: Failed to get cmd\n");
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;

	memset(ci, 0, sizeof(*ci));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->mbox.b[0] = query_type;
	if (instance->supportmax256vd)
		dcmd->mbox.b[2] = 1;

	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = MFI_STAT_INVALID_STATUS;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->data_xfer_len = cpu_to_le32(sizeof(struct MR_LD_TARGETID_LIST));
	dcmd->opcode = cpu_to_le32(MR_DCMD_LD_LIST_QUERY);
	dcmd->pad_0  = 0;

	megasas_set_dma_settings(instance, dcmd, ci_h,
				 sizeof(struct MR_LD_TARGETID_LIST));

	if ((instance->adapter_type != MFI_SERIES) &&
	    !instance->mask_interrupts)
		ret = megasas_issue_blocked_cmd(instance, cmd, MFI_IO_TIMEOUT_SECS);
	else
		ret = megasas_issue_polled(instance, cmd);

	switch (ret) {
	case DCMD_FAILED:
		dev_info(&instance->pdev->dev,
			"DCMD not supported by firmware - %s %d\n",
				__func__, __LINE__);
		ret = megasas_get_ld_list(instance);
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
		tgtid_count = le32_to_cpu(ci->count);

		if ((tgtid_count > (instance->fw_supported_vd_count)))
			break;

		memset(instance->ld_ids, 0xff, MEGASAS_MAX_LD_IDS);
		for (ld_index = 0; ld_index < tgtid_count; ld_index++) {
			ids = ci->targetId[ld_index];
			instance->ld_ids[ids] = ci->targetId[ld_index];
		}

		break;
	}

	if (ret != DCMD_TIMEOUT)
		megasas_return_cmd(instance, cmd);

	return ret;
}
