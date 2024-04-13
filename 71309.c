megasas_get_seq_num(struct megasas_instance *instance,
		    struct megasas_evt_log_info *eli)
{
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	struct megasas_evt_log_info *el_info;
	dma_addr_t el_info_h = 0;
	int ret;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;
	el_info = dma_zalloc_coherent(&instance->pdev->dev,
			sizeof(struct megasas_evt_log_info), &el_info_h,
			GFP_KERNEL);
	if (!el_info) {
		megasas_return_cmd(instance, cmd);
		return -ENOMEM;
	}

	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = 0x0;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len = cpu_to_le32(sizeof(struct megasas_evt_log_info));
	dcmd->opcode = cpu_to_le32(MR_DCMD_CTRL_EVENT_GET_INFO);

	megasas_set_dma_settings(instance, dcmd, el_info_h,
				 sizeof(struct megasas_evt_log_info));

	ret = megasas_issue_blocked_cmd(instance, cmd, MFI_IO_TIMEOUT_SECS);
	if (ret != DCMD_SUCCESS) {
		dev_err(&instance->pdev->dev, "Failed from %s %d\n",
			__func__, __LINE__);
		goto dcmd_failed;
	}

	/*
	 * Copy the data back into callers buffer
	 */
	eli->newest_seq_num = el_info->newest_seq_num;
	eli->oldest_seq_num = el_info->oldest_seq_num;
	eli->clear_seq_num = el_info->clear_seq_num;
	eli->shutdown_seq_num = el_info->shutdown_seq_num;
	eli->boot_seq_num = el_info->boot_seq_num;

dcmd_failed:
	dma_free_coherent(&instance->pdev->dev,
			sizeof(struct megasas_evt_log_info),
			el_info, el_info_h);

	megasas_return_cmd(instance, cmd);

	return ret;
}
