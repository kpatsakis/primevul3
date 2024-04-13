megasas_return_cmd(struct megasas_instance *instance, struct megasas_cmd *cmd)
{
	unsigned long flags;
	u32 blk_tags;
	struct megasas_cmd_fusion *cmd_fusion;
	struct fusion_context *fusion = instance->ctrl_context;

	/* This flag is used only for fusion adapter.
	 * Wait for Interrupt for Polled mode DCMD
	 */
	if (cmd->flags & DRV_DCMD_POLLED_MODE)
		return;

	spin_lock_irqsave(&instance->mfi_pool_lock, flags);

	if (fusion) {
		blk_tags = instance->max_scsi_cmds + cmd->index;
		cmd_fusion = fusion->cmd_list[blk_tags];
		megasas_return_cmd_fusion(instance, cmd_fusion);
	}
	cmd->scmd = NULL;
	cmd->frame_count = 0;
	cmd->flags = 0;
	memset(cmd->frame, 0, instance->mfi_frame_size);
	cmd->frame->io.context = cpu_to_le32(cmd->index);
	if (!fusion && reset_devices)
		cmd->frame->hdr.cmd = MFI_CMD_INVALID;
	list_add(&cmd->list, (&instance->cmd_pool)->next);

	spin_unlock_irqrestore(&instance->mfi_pool_lock, flags);

}
