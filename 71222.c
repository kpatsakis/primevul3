static enum si_sm_result start_next_msg(struct smi_info *smi_info)
{
	int              rv;

	if (!smi_info->waiting_msg) {
		smi_info->curr_msg = NULL;
		rv = SI_SM_IDLE;
	} else {
		int err;

		smi_info->curr_msg = smi_info->waiting_msg;
		smi_info->waiting_msg = NULL;
		debug_timestamp("Start2");
		err = atomic_notifier_call_chain(&xaction_notifier_list,
				0, smi_info);
		if (err & NOTIFY_STOP_MASK) {
			rv = SI_SM_CALL_WITHOUT_DELAY;
			goto out;
		}
		err = smi_info->handlers->start_transaction(
			smi_info->si_sm,
			smi_info->curr_msg->data,
			smi_info->curr_msg->data_size);
		if (err)
			return_hosed_msg(smi_info, err);

		rv = SI_SM_CALL_WITHOUT_DELAY;
	}
out:
	return rv;
}
