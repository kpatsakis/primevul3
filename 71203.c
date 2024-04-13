static void return_hosed_msg_badsize(struct smi_info *smi_info)
{
	struct ipmi_smi_msg *msg = smi_info->curr_msg;

	/* Make it a response */
	msg->rsp[0] = msg->data[0] | 4;
	msg->rsp[1] = msg->data[1];
	msg->rsp[2] = CANNOT_RETURN_REQUESTED_LENGTH;
	msg->rsp_size = 3;
	smi_info->curr_msg = NULL;
	deliver_recv_msg(smi_info, msg);
}
