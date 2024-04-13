megasas_complete_cmd(struct megasas_instance *instance, struct megasas_cmd *cmd,
		     u8 alt_status)
{
	int exception = 0;
	struct megasas_header *hdr = &cmd->frame->hdr;
	unsigned long flags;
	struct fusion_context *fusion = instance->ctrl_context;
	u32 opcode, status;

	/* flag for the retry reset */
	cmd->retry_for_fw_reset = 0;

	if (cmd->scmd)
		cmd->scmd->SCp.ptr = NULL;

	switch (hdr->cmd) {
	case MFI_CMD_INVALID:
		/* Some older 1068 controller FW may keep a pended
		   MR_DCMD_CTRL_EVENT_GET_INFO left over from the main kernel
		   when booting the kdump kernel.  Ignore this command to
		   prevent a kernel panic on shutdown of the kdump kernel. */
		dev_warn(&instance->pdev->dev, "MFI_CMD_INVALID command "
		       "completed\n");
		dev_warn(&instance->pdev->dev, "If you have a controller "
		       "other than PERC5, please upgrade your firmware\n");
		break;
	case MFI_CMD_PD_SCSI_IO:
	case MFI_CMD_LD_SCSI_IO:

		/*
		 * MFI_CMD_PD_SCSI_IO and MFI_CMD_LD_SCSI_IO could have been
		 * issued either through an IO path or an IOCTL path. If it
		 * was via IOCTL, we will send it to internal completion.
		 */
		if (cmd->sync_cmd) {
			cmd->sync_cmd = 0;
			megasas_complete_int_cmd(instance, cmd);
			break;
		}
		/* fall through */

	case MFI_CMD_LD_READ:
	case MFI_CMD_LD_WRITE:

		if (alt_status) {
			cmd->scmd->result = alt_status << 16;
			exception = 1;
		}

		if (exception) {

			atomic_dec(&instance->fw_outstanding);

			scsi_dma_unmap(cmd->scmd);
			cmd->scmd->scsi_done(cmd->scmd);
			megasas_return_cmd(instance, cmd);

			break;
		}

		switch (hdr->cmd_status) {

		case MFI_STAT_OK:
			cmd->scmd->result = DID_OK << 16;
			break;

		case MFI_STAT_SCSI_IO_FAILED:
		case MFI_STAT_LD_INIT_IN_PROGRESS:
			cmd->scmd->result =
			    (DID_ERROR << 16) | hdr->scsi_status;
			break;

		case MFI_STAT_SCSI_DONE_WITH_ERROR:

			cmd->scmd->result = (DID_OK << 16) | hdr->scsi_status;

			if (hdr->scsi_status == SAM_STAT_CHECK_CONDITION) {
				memset(cmd->scmd->sense_buffer, 0,
				       SCSI_SENSE_BUFFERSIZE);
				memcpy(cmd->scmd->sense_buffer, cmd->sense,
				       hdr->sense_len);

				cmd->scmd->result |= DRIVER_SENSE << 24;
			}

			break;

		case MFI_STAT_LD_OFFLINE:
		case MFI_STAT_DEVICE_NOT_FOUND:
			cmd->scmd->result = DID_BAD_TARGET << 16;
			break;

		default:
			dev_printk(KERN_DEBUG, &instance->pdev->dev, "MFI FW status %#x\n",
			       hdr->cmd_status);
			cmd->scmd->result = DID_ERROR << 16;
			break;
		}

		atomic_dec(&instance->fw_outstanding);

		scsi_dma_unmap(cmd->scmd);
		cmd->scmd->scsi_done(cmd->scmd);
		megasas_return_cmd(instance, cmd);

		break;

	case MFI_CMD_SMP:
	case MFI_CMD_STP:
	case MFI_CMD_NVME:
		megasas_complete_int_cmd(instance, cmd);
		break;

	case MFI_CMD_DCMD:
		opcode = le32_to_cpu(cmd->frame->dcmd.opcode);
		/* Check for LD map update */
		if ((opcode == MR_DCMD_LD_MAP_GET_INFO)
			&& (cmd->frame->dcmd.mbox.b[1] == 1)) {
			fusion->fast_path_io = 0;
			spin_lock_irqsave(instance->host->host_lock, flags);
			status = cmd->frame->hdr.cmd_status;
			instance->map_update_cmd = NULL;
			if (status != MFI_STAT_OK) {
				if (status != MFI_STAT_NOT_FOUND)
					dev_warn(&instance->pdev->dev, "map syncfailed, status = 0x%x\n",
					       cmd->frame->hdr.cmd_status);
				else {
					megasas_return_cmd(instance, cmd);
					spin_unlock_irqrestore(
						instance->host->host_lock,
						flags);
					break;
				}
			}

			megasas_return_cmd(instance, cmd);

			/*
			 * Set fast path IO to ZERO.
			 * Validate Map will set proper value.
			 * Meanwhile all IOs will go as LD IO.
			 */
			if (status == MFI_STAT_OK &&
			    (MR_ValidateMapInfo(instance, (instance->map_id + 1)))) {
				instance->map_id++;
				fusion->fast_path_io = 1;
			} else {
				fusion->fast_path_io = 0;
			}

			megasas_sync_map_info(instance);
			spin_unlock_irqrestore(instance->host->host_lock,
					       flags);
			break;
		}
		if (opcode == MR_DCMD_CTRL_EVENT_GET_INFO ||
		    opcode == MR_DCMD_CTRL_EVENT_GET) {
			spin_lock_irqsave(&poll_aen_lock, flags);
			megasas_poll_wait_aen = 0;
			spin_unlock_irqrestore(&poll_aen_lock, flags);
		}

		/* FW has an updated PD sequence */
		if ((opcode == MR_DCMD_SYSTEM_PD_MAP_GET_INFO) &&
			(cmd->frame->dcmd.mbox.b[0] == 1)) {

			spin_lock_irqsave(instance->host->host_lock, flags);
			status = cmd->frame->hdr.cmd_status;
			instance->jbod_seq_cmd = NULL;
			megasas_return_cmd(instance, cmd);

			if (status == MFI_STAT_OK) {
				instance->pd_seq_map_id++;
				/* Re-register a pd sync seq num cmd */
				if (megasas_sync_pd_seq_num(instance, true))
					instance->use_seqnum_jbod_fp = false;
			} else
				instance->use_seqnum_jbod_fp = false;

			spin_unlock_irqrestore(instance->host->host_lock, flags);
			break;
		}

		/*
		 * See if got an event notification
		 */
		if (opcode == MR_DCMD_CTRL_EVENT_WAIT)
			megasas_service_aen(instance, cmd);
		else
			megasas_complete_int_cmd(instance, cmd);

		break;

	case MFI_CMD_ABORT:
		/*
		 * Cmd issued to abort another cmd returned
		 */
		megasas_complete_abort(instance, cmd);
		break;

	default:
		dev_info(&instance->pdev->dev, "Unknown command completed! [0x%X]\n",
		       hdr->cmd);
		megasas_complete_int_cmd(instance, cmd);
		break;
	}
}
