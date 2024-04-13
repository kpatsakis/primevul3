static inline bool disable_si_irq(struct smi_info *smi_info)
{
	if ((smi_info->io.irq) && (!smi_info->interrupt_disabled)) {
		smi_info->interrupt_disabled = true;
		start_check_enables(smi_info);
		return true;
	}
	return false;
}
