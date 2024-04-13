static void start_check_enables(struct smi_info *smi_info)
{
	unsigned char msg[2];

	msg[0] = (IPMI_NETFN_APP_REQUEST << 2);
	msg[1] = IPMI_GET_BMC_GLOBAL_ENABLES_CMD;

	start_new_msg(smi_info, msg, 2);
	smi_info->si_state = SI_CHECKING_ENABLES;
}
