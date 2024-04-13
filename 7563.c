int ccp_run_cmd(struct ccp_cmd_queue *cmd_q, struct ccp_cmd *cmd)
{
	int ret;

	cmd->engine_error = 0;
	cmd_q->cmd_error = 0;
	cmd_q->int_rcvd = 0;
	cmd_q->free_slots = cmd_q->ccp->vdata->perform->get_free_slots(cmd_q);

	switch (cmd->engine) {
	case CCP_ENGINE_AES:
		switch (cmd->u.aes.mode) {
		case CCP_AES_MODE_CMAC:
			ret = ccp_run_aes_cmac_cmd(cmd_q, cmd);
			break;
		case CCP_AES_MODE_GCM:
			ret = ccp_run_aes_gcm_cmd(cmd_q, cmd);
			break;
		default:
			ret = ccp_run_aes_cmd(cmd_q, cmd);
			break;
		}
		break;
	case CCP_ENGINE_XTS_AES_128:
		ret = ccp_run_xts_aes_cmd(cmd_q, cmd);
		break;
	case CCP_ENGINE_DES3:
		ret = ccp_run_des3_cmd(cmd_q, cmd);
		break;
	case CCP_ENGINE_SHA:
		ret = ccp_run_sha_cmd(cmd_q, cmd);
		break;
	case CCP_ENGINE_RSA:
		ret = ccp_run_rsa_cmd(cmd_q, cmd);
		break;
	case CCP_ENGINE_PASSTHRU:
		if (cmd->flags & CCP_CMD_PASSTHRU_NO_DMA_MAP)
			ret = ccp_run_passthru_nomap_cmd(cmd_q, cmd);
		else
			ret = ccp_run_passthru_cmd(cmd_q, cmd);
		break;
	case CCP_ENGINE_ECC:
		ret = ccp_run_ecc_cmd(cmd_q, cmd);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}