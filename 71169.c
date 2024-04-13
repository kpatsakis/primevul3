static void check_bt_irq(struct smi_info *smi_info, bool irq_on)
{
	u8 irqstate = smi_info->io.inputb(&smi_info->io, IPMI_BT_INTMASK_REG);

	irqstate &= IPMI_BT_INTMASK_ENABLE_IRQ_BIT;

	if ((bool)irqstate == irq_on)
		return;

	if (irq_on)
		smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG,
				     IPMI_BT_INTMASK_ENABLE_IRQ_BIT);
	else
		smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG, 0);
}
