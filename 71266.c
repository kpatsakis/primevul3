megasas_complete_abort(struct megasas_instance *instance,
		       struct megasas_cmd *cmd)
{
	if (cmd->sync_cmd) {
		cmd->sync_cmd = 0;
		cmd->cmd_status_drv = 0;
		wake_up(&instance->abort_cmd_wait_q);
	}
}
