megasas_host_device_list_query(struct megasas_instance *instance,
			       bool is_probe)
{
	int ret, i, target_id;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	struct MR_HOST_DEVICE_LIST *ci;
	u32 count;
	dma_addr_t ci_h;

	ci = instance->host_device_list_buf;
	ci_h = instance->host_device_list_buf_h;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_warn(&instance->pdev->dev,
			 "%s: failed to get cmd\n",
			 __func__);
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;

	memset(ci, 0, sizeof(*ci));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->mbox.b[0] = is_probe ? 0 : 1;
	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = MFI_STAT_INVALID_STATUS;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len = cpu_to_le32(HOST_DEVICE_LIST_SZ);
	dcmd->opcode = cpu_to_le32(MR_DCMD_CTRL_DEVICE_LIST_GET);

	megasas_set_dma_settings(instance, dcmd, ci_h, HOST_DEVICE_LIST_SZ);

	if (!instance->mask_interrupts) {
		ret = megasas_issue_blocked_cmd(instance, cmd,
						MFI_IO_TIMEOUT_SECS);
	} else {
		ret = megasas_issue_polled(instance, cmd);
		cmd->flags |= DRV_DCMD_SKIP_REFIRE;
	}

	switch (ret) {
	case DCMD_SUCCESS:
		/* Fill the internal pd_list and ld_ids array based on
		 * targetIds returned by FW
		 */
		count = le32_to_cpu(ci->count);

		memset(instance->local_pd_list, 0,
		       MEGASAS_MAX_PD * sizeof(struct megasas_pd_list));
		memset(instance->ld_ids, 0xff, MAX_LOGICAL_DRIVES_EXT);
		for (i = 0; i < count; i++) {
			target_id = le16_to_cpu(ci->host_device_list[i].target_id);
			if (ci->host_device_list[i].flags.u.bits.is_sys_pd) {
				instance->local_pd_list[target_id].tid = target_id;
				instance->local_pd_list[target_id].driveType =
						ci->host_device_list[i].scsi_type;
				instance->local_pd_list[target_id].driveState =
						MR_PD_STATE_SYSTEM;
			} else {
				instance->ld_ids[target_id] = target_id;
			}
		}

		memcpy(instance->pd_list, instance->local_pd_list,
		       sizeof(instance->pd_list));
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
	case DCMD_FAILED:
		dev_err(&instance->pdev->dev,
			"%s: MR_DCMD_CTRL_DEVICE_LIST_GET failed\n",
			__func__);
		break;
	}

	if (ret != DCMD_TIMEOUT)
		megasas_return_cmd(instance, cmd);

	return ret;
}
