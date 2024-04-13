megasas_get_ctrl_info(struct megasas_instance *instance)
{
	int ret = 0;
	struct megasas_cmd *cmd;
	struct megasas_dcmd_frame *dcmd;
	struct megasas_ctrl_info *ci;
	dma_addr_t ci_h = 0;

	ci = instance->ctrl_info_buf;
	ci_h = instance->ctrl_info_buf_h;

	cmd = megasas_get_cmd(instance);

	if (!cmd) {
		dev_printk(KERN_DEBUG, &instance->pdev->dev, "Failed to get a free cmd\n");
		return -ENOMEM;
	}

	dcmd = &cmd->frame->dcmd;

	memset(ci, 0, sizeof(*ci));
	memset(dcmd->mbox.b, 0, MFI_MBOX_SIZE);

	dcmd->cmd = MFI_CMD_DCMD;
	dcmd->cmd_status = MFI_STAT_INVALID_STATUS;
	dcmd->sge_count = 1;
	dcmd->flags = MFI_FRAME_DIR_READ;
	dcmd->timeout = 0;
	dcmd->pad_0 = 0;
	dcmd->data_xfer_len = cpu_to_le32(sizeof(struct megasas_ctrl_info));
	dcmd->opcode = cpu_to_le32(MR_DCMD_CTRL_GET_INFO);
	dcmd->mbox.b[0] = 1;

	megasas_set_dma_settings(instance, dcmd, ci_h,
				 sizeof(struct megasas_ctrl_info));

	if ((instance->adapter_type != MFI_SERIES) &&
	    !instance->mask_interrupts) {
		ret = megasas_issue_blocked_cmd(instance, cmd, MFI_IO_TIMEOUT_SECS);
	} else {
		ret = megasas_issue_polled(instance, cmd);
		cmd->flags |= DRV_DCMD_SKIP_REFIRE;
	}

	switch (ret) {
	case DCMD_SUCCESS:
		/* Save required controller information in
		 * CPU endianness format.
		 */
		le32_to_cpus((u32 *)&ci->properties.OnOffProperties);
		le16_to_cpus((u16 *)&ci->properties.on_off_properties2);
		le32_to_cpus((u32 *)&ci->adapterOperations2);
		le32_to_cpus((u32 *)&ci->adapterOperations3);
		le16_to_cpus((u16 *)&ci->adapter_operations4);

		/* Update the latest Ext VD info.
		 * From Init path, store current firmware details.
		 * From OCR path, detect any firmware properties changes.
		 * in case of Firmware upgrade without system reboot.
		 */
		megasas_update_ext_vd_details(instance);
		instance->use_seqnum_jbod_fp =
			ci->adapterOperations3.useSeqNumJbodFP;
		instance->support_morethan256jbod =
			ci->adapter_operations4.support_pd_map_target_id;
		instance->support_nvme_passthru =
			ci->adapter_operations4.support_nvme_passthru;
		instance->task_abort_tmo = ci->TaskAbortTO;
		instance->max_reset_tmo = ci->MaxResetTO;

		/*Check whether controller is iMR or MR */
		instance->is_imr = (ci->memory_size ? 0 : 1);

		instance->snapdump_wait_time =
			(ci->properties.on_off_properties2.enable_snap_dump ?
			 MEGASAS_DEFAULT_SNAP_DUMP_WAIT_TIME : 0);

		instance->enable_fw_dev_list =
			ci->properties.on_off_properties2.enable_fw_dev_list;

		dev_info(&instance->pdev->dev,
			"controller type\t: %s(%dMB)\n",
			instance->is_imr ? "iMR" : "MR",
			le16_to_cpu(ci->memory_size));

		instance->disableOnlineCtrlReset =
			ci->properties.OnOffProperties.disableOnlineCtrlReset;
		instance->secure_jbod_support =
			ci->adapterOperations3.supportSecurityonJBOD;
		dev_info(&instance->pdev->dev, "Online Controller Reset(OCR)\t: %s\n",
			instance->disableOnlineCtrlReset ? "Disabled" : "Enabled");
		dev_info(&instance->pdev->dev, "Secure JBOD support\t: %s\n",
			instance->secure_jbod_support ? "Yes" : "No");
		dev_info(&instance->pdev->dev, "NVMe passthru support\t: %s\n",
			 instance->support_nvme_passthru ? "Yes" : "No");
		dev_info(&instance->pdev->dev,
			 "FW provided TM TaskAbort/Reset timeout\t: %d secs/%d secs\n",
			 instance->task_abort_tmo, instance->max_reset_tmo);

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
		megaraid_sas_kill_hba(instance);
		break;

	}

	if (ret != DCMD_TIMEOUT)
		megasas_return_cmd(instance, cmd);

	return ret;
}
