megasas_read_fw_status_reg_skinny(struct megasas_instance *instance)
{
	return readl(&instance->reg_set->outbound_scratch_pad_0);
}
