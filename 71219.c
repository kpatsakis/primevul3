static void start_getting_events(struct smi_info *smi_info)
{
	smi_info->curr_msg->data[0] = (IPMI_NETFN_APP_REQUEST << 2);
	smi_info->curr_msg->data[1] = IPMI_READ_EVENT_MSG_BUFFER_CMD;
	smi_info->curr_msg->data_size = 2;

	start_new_msg(smi_info, smi_info->curr_msg->data,
		      smi_info->curr_msg->data_size);
	smi_info->si_state = SI_GETTING_EVENTS;
}
