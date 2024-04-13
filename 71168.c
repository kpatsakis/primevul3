static struct ipmi_smi_msg *alloc_msg_handle_irq(struct smi_info *smi_info)
{
	struct ipmi_smi_msg *msg;

	msg = ipmi_alloc_smi_msg();
	if (!msg) {
		if (!disable_si_irq(smi_info))
			smi_info->si_state = SI_NORMAL;
	} else if (enable_si_irq(smi_info)) {
		ipmi_free_smi_msg(msg);
		msg = NULL;
	}
	return msg;
}
