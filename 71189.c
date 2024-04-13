irqreturn_t ipmi_si_irq_handler(int irq, void *data)
{
	struct smi_info *smi_info = data;
	unsigned long   flags;

	if (smi_info->io.si_type == SI_BT)
		/* We need to clear the IRQ flag for the BT interface. */
		smi_info->io.outputb(&smi_info->io, IPMI_BT_INTMASK_REG,
				     IPMI_BT_INTMASK_CLEAR_IRQ_BIT
				     | IPMI_BT_INTMASK_ENABLE_IRQ_BIT);

	spin_lock_irqsave(&(smi_info->si_lock), flags);

	smi_inc_stat(smi_info, interrupts);

	debug_timestamp("Interrupt");

	smi_event_handler(smi_info, 0);
	spin_unlock_irqrestore(&(smi_info->si_lock), flags);
	return IRQ_HANDLED;
}
