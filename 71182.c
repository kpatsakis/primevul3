static void handle_flags(struct smi_info *smi_info)
{
retry:
	if (smi_info->msg_flags & WDT_PRE_TIMEOUT_INT) {
		/* Watchdog pre-timeout */
		smi_inc_stat(smi_info, watchdog_pretimeouts);

		start_clear_flags(smi_info);
		smi_info->msg_flags &= ~WDT_PRE_TIMEOUT_INT;
		ipmi_smi_watchdog_pretimeout(smi_info->intf);
	} else if (smi_info->msg_flags & RECEIVE_MSG_AVAIL) {
		/* Messages available. */
		smi_info->curr_msg = alloc_msg_handle_irq(smi_info);
		if (!smi_info->curr_msg)
			return;

		start_getting_msg_queue(smi_info);
	} else if (smi_info->msg_flags & EVENT_MSG_BUFFER_FULL) {
		/* Events available. */
		smi_info->curr_msg = alloc_msg_handle_irq(smi_info);
		if (!smi_info->curr_msg)
			return;

		start_getting_events(smi_info);
	} else if (smi_info->msg_flags & OEM_DATA_AVAIL &&
		   smi_info->oem_data_avail_handler) {
		if (smi_info->oem_data_avail_handler(smi_info))
			goto retry;
	} else
		smi_info->si_state = SI_NORMAL;
}
