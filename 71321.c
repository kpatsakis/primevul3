megasas_issue_dcmd(struct megasas_instance *instance, struct megasas_cmd *cmd)
{
	instance->instancet->fire_cmd(instance,
		cmd->frame_phys_addr, 0, instance->reg_set);
	return;
}
