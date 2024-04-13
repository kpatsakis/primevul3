static void return_hosed_msg(struct smi_info *smi_info, int cCode)
{
	struct ipmi_smi_msg *msg = smi_info->curr_msg;

	if (cCode < 0 || cCode > IPMI_ERR_UNSPECIFIED)
		cCode = IPMI_ERR_UNSPECIFIED;
	/* else use it as is */

	/* Make it a response */
	msg->rsp[0] = msg->data[0] | 4;
	msg->rsp[1] = msg->data[1];
	msg->rsp[2] = cCode;
	msg->rsp_size = 3;

	smi_info->curr_msg = NULL;
	deliver_recv_msg(smi_info, msg);
}
