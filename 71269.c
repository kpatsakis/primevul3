megasas_complete_int_cmd(struct megasas_instance *instance,
			 struct megasas_cmd *cmd)
{
	cmd->cmd_status_drv = cmd->frame->io.cmd_status;
	wake_up(&instance->int_cmd_wait_q);
}
