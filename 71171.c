static void check_for_broken_irqs(struct smi_info *smi_info)
{
	check_clr_rcv_irq(smi_info);
	check_set_rcv_irq(smi_info);
}
