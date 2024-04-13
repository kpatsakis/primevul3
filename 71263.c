megasas_clear_intr_gen2(struct megasas_instance *instance)
{
	u32 status;
	u32 mfiStatus = 0;
	struct megasas_register_set __iomem *regs;
	regs = instance->reg_set;

	/*
	 * Check if it is our interrupt
	 */
	status = readl(&regs->outbound_intr_status);

	if (status & MFI_INTR_FLAG_REPLY_MESSAGE) {
		mfiStatus = MFI_INTR_FLAG_REPLY_MESSAGE;
	}
	if (status & MFI_G2_OUTBOUND_DOORBELL_CHANGE_INTERRUPT) {
		mfiStatus |= MFI_INTR_FLAG_FIRMWARE_STATE_CHANGE;
	}

	/*
	 * Clear the interrupt by writing back the same value
	 */
	if (mfiStatus)
		writel(status, &regs->outbound_doorbell_clear);

	/* Dummy readl to force pci flush */
	readl(&regs->outbound_intr_status);

	return mfiStatus;
}
