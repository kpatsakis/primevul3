static u8 current_global_enables(struct smi_info *smi_info, u8 base,
				 bool *irq_on)
{
	u8 enables = 0;

	if (smi_info->supports_event_msg_buff)
		enables |= IPMI_BMC_EVT_MSG_BUFF;

	if (((smi_info->io.irq && !smi_info->interrupt_disabled) ||
	     smi_info->cannot_disable_irq) &&
	    !smi_info->irq_enable_broken)
		enables |= IPMI_BMC_RCV_MSG_INTR;

	if (smi_info->supports_event_msg_buff &&
	    smi_info->io.irq && !smi_info->interrupt_disabled &&
	    !smi_info->irq_enable_broken)
		enables |= IPMI_BMC_EVT_MSG_INTR;

	*irq_on = enables & (IPMI_BMC_EVT_MSG_INTR | IPMI_BMC_RCV_MSG_INTR);

	return enables;
}
