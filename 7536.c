ccp_run_ecc_cmd(struct ccp_cmd_queue *cmd_q, struct ccp_cmd *cmd)
{
	struct ccp_ecc_engine *ecc = &cmd->u.ecc;

	ecc->ecc_result = 0;

	if (!ecc->mod ||
	    (ecc->mod_len > CCP_ECC_MODULUS_BYTES))
		return -EINVAL;

	switch (ecc->function) {
	case CCP_ECC_FUNCTION_MMUL_384BIT:
	case CCP_ECC_FUNCTION_MADD_384BIT:
	case CCP_ECC_FUNCTION_MINV_384BIT:
		return ccp_run_ecc_mm_cmd(cmd_q, cmd);

	case CCP_ECC_FUNCTION_PADD_384BIT:
	case CCP_ECC_FUNCTION_PMUL_384BIT:
	case CCP_ECC_FUNCTION_PDBL_384BIT:
		return ccp_run_ecc_pm_cmd(cmd_q, cmd);

	default:
		return -EINVAL;
	}
}