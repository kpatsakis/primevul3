megasas_read_fw_status_reg_xscale(struct megasas_instance *instance)
{
	return readl(&instance->reg_set->outbound_msg_0);
}
