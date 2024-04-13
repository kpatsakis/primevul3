megasas_build_dcdb(struct megasas_instance *instance, struct scsi_cmnd *scp,
		   struct megasas_cmd *cmd)
{
	u32 is_logical;
	u32 device_id;
	u16 flags = 0;
	struct megasas_pthru_frame *pthru;

	is_logical = MEGASAS_IS_LOGICAL(scp->device);
	device_id = MEGASAS_DEV_INDEX(scp);
	pthru = (struct megasas_pthru_frame *)cmd->frame;

	if (scp->sc_data_direction == DMA_TO_DEVICE)
		flags = MFI_FRAME_DIR_WRITE;
	else if (scp->sc_data_direction == DMA_FROM_DEVICE)
		flags = MFI_FRAME_DIR_READ;
	else if (scp->sc_data_direction == DMA_NONE)
		flags = MFI_FRAME_DIR_NONE;

	if (instance->flag_ieee == 1) {
		flags |= MFI_FRAME_IEEE;
	}

	/*
	 * Prepare the DCDB frame
	 */
	pthru->cmd = (is_logical) ? MFI_CMD_LD_SCSI_IO : MFI_CMD_PD_SCSI_IO;
	pthru->cmd_status = 0x0;
	pthru->scsi_status = 0x0;
	pthru->target_id = device_id;
	pthru->lun = scp->device->lun;
	pthru->cdb_len = scp->cmd_len;
	pthru->timeout = 0;
	pthru->pad_0 = 0;
	pthru->flags = cpu_to_le16(flags);
	pthru->data_xfer_len = cpu_to_le32(scsi_bufflen(scp));

	memcpy(pthru->cdb, scp->cmnd, scp->cmd_len);

	/*
	 * If the command is for the tape device, set the
	 * pthru timeout to the os layer timeout value.
	 */
	if (scp->device->type == TYPE_TAPE) {
		if ((scp->request->timeout / HZ) > 0xFFFF)
			pthru->timeout = cpu_to_le16(0xFFFF);
		else
			pthru->timeout = cpu_to_le16(scp->request->timeout / HZ);
	}

	/*
	 * Construct SGL
	 */
	if (instance->flag_ieee == 1) {
		pthru->flags |= cpu_to_le16(MFI_FRAME_SGL64);
		pthru->sge_count = megasas_make_sgl_skinny(instance, scp,
						      &pthru->sgl);
	} else if (IS_DMA64) {
		pthru->flags |= cpu_to_le16(MFI_FRAME_SGL64);
		pthru->sge_count = megasas_make_sgl64(instance, scp,
						      &pthru->sgl);
	} else
		pthru->sge_count = megasas_make_sgl32(instance, scp,
						      &pthru->sgl);

	if (pthru->sge_count > instance->max_num_sge) {
		dev_err(&instance->pdev->dev, "DCDB too many SGE NUM=%x\n",
			pthru->sge_count);
		return 0;
	}

	/*
	 * Sense info specific
	 */
	pthru->sense_len = SCSI_SENSE_BUFFERSIZE;
	pthru->sense_buf_phys_addr_hi =
		cpu_to_le32(upper_32_bits(cmd->sense_phys_addr));
	pthru->sense_buf_phys_addr_lo =
		cpu_to_le32(lower_32_bits(cmd->sense_phys_addr));

	/*
	 * Compute the total number of frames this command consumes. FW uses
	 * this number to pull sufficient number of frames from host memory.
	 */
	cmd->frame_count = megasas_get_frame_count(instance, pthru->sge_count,
							PTHRU_FRAME);

	return cmd->frame_count;
}
