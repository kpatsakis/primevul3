megasas_enable_intr_gen2(struct megasas_instance *instance)
{
	struct megasas_register_set __iomem *regs;

	regs = instance->reg_set;
	writel(0xFFFFFFFF, &(regs)->outbound_doorbell_clear);

	/* write ~0x00000005 (4 & 1) to the intr mask*/
	writel(~MFI_GEN2_ENABLE_INTERRUPT_MASK, &(regs)->outbound_intr_mask);

	/* Dummy readl to force pci flush */
	readl(&regs->outbound_intr_mask);
}
