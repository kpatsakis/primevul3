megasas_clear_intr_ppc(struct megasas_instance *instance)
{
	u32 status, mfiStatus = 0;
	struct megasas_register_set __iomem *regs;
	regs = instance->reg_set;

	/*
	 * Check if it is our interrupt
	 */
	status = readl(&regs->outbound_intr_status);

	if (status & MFI_REPLY_1078_MESSAGE_INTERRUPT)
		mfiStatus = MFI_INTR_FLAG_REPLY_MESSAGE;

	if (status & MFI_G2_OUTBOUND_DOORBELL_CHANGE_INTERRUPT)
		mfiStatus |= MFI_INTR_FLAG_FIRMWARE_STATE_CHANGE;

	/*
	 * Clear the interrupt by writing back the same value
	 */
	writel(status, &regs->outbound_doorbell_clear);

	/* Dummy readl to force pci flush */
	readl(&regs->outbound_doorbell_clear);

	return mfiStatus;
}
