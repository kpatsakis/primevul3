megasas_issue_init_mfi(struct megasas_instance *instance)
{
	__le32 context;
	struct megasas_cmd *cmd;
	struct megasas_init_frame *init_frame;
	struct megasas_init_queue_info *initq_info;
	dma_addr_t init_frame_h;
	dma_addr_t initq_info_h;

	/*
	 * Prepare a init frame. Note the init frame points to queue info
	 * structure. Each frame has SGL allocated after first 64 bytes. For
	 * this frame - since we don't need any SGL - we use SGL's space as
	 * queue info structure
	 *
	 * We will not get a NULL command below. We just created the pool.
	 */
	cmd = megasas_get_cmd(instance);

	init_frame = (struct megasas_init_frame *)cmd->frame;
	initq_info = (struct megasas_init_queue_info *)
		((unsigned long)init_frame + 64);

	init_frame_h = cmd->frame_phys_addr;
	initq_info_h = init_frame_h + 64;

	context = init_frame->context;
	memset(init_frame, 0, MEGAMFI_FRAME_SIZE);
	memset(initq_info, 0, sizeof(struct megasas_init_queue_info));
	init_frame->context = context;

	initq_info->reply_queue_entries = cpu_to_le32(instance->max_fw_cmds + 1);
	initq_info->reply_queue_start_phys_addr_lo = cpu_to_le32(instance->reply_queue_h);

	initq_info->producer_index_phys_addr_lo = cpu_to_le32(instance->producer_h);
	initq_info->consumer_index_phys_addr_lo = cpu_to_le32(instance->consumer_h);

	init_frame->cmd = MFI_CMD_INIT;
	init_frame->cmd_status = MFI_STAT_INVALID_STATUS;
	init_frame->queue_info_new_phys_addr_lo =
		cpu_to_le32(lower_32_bits(initq_info_h));
	init_frame->queue_info_new_phys_addr_hi =
		cpu_to_le32(upper_32_bits(initq_info_h));

	init_frame->data_xfer_len = cpu_to_le32(sizeof(struct megasas_init_queue_info));

	/*
	 * disable the intr before firing the init frame to FW
	 */
	instance->instancet->disable_intr(instance);

	/*
	 * Issue the init frame in polled mode
	 */

	if (megasas_issue_polled(instance, cmd)) {
		dev_err(&instance->pdev->dev, "Failed to init firmware\n");
		megasas_return_cmd(instance, cmd);
		goto fail_fw_init;
	}

	megasas_return_cmd(instance, cmd);

	return 0;

fail_fw_init:
	return -EINVAL;
}
