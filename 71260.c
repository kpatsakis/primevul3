megasas_check_reset_ppc(struct megasas_instance *instance,
			struct megasas_register_set __iomem *regs)
{
	if (atomic_read(&instance->adprecovery) != MEGASAS_HBA_OPERATIONAL)
		return 1;

	return 0;
}
