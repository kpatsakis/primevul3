static int ccp_run_ecc_mm_cmd(struct ccp_cmd_queue *cmd_q, struct ccp_cmd *cmd)
{
	struct ccp_ecc_engine *ecc = &cmd->u.ecc;
	struct ccp_dm_workarea src, dst;
	struct ccp_op op;
	int ret;
	u8 *save;

	if (!ecc->u.mm.operand_1 ||
	    (ecc->u.mm.operand_1_len > CCP_ECC_MODULUS_BYTES))
		return -EINVAL;

	if (ecc->function != CCP_ECC_FUNCTION_MINV_384BIT)
		if (!ecc->u.mm.operand_2 ||
		    (ecc->u.mm.operand_2_len > CCP_ECC_MODULUS_BYTES))
			return -EINVAL;

	if (!ecc->u.mm.result ||
	    (ecc->u.mm.result_len < CCP_ECC_MODULUS_BYTES))
		return -EINVAL;

	memset(&op, 0, sizeof(op));
	op.cmd_q = cmd_q;
	op.jobid = CCP_NEW_JOBID(cmd_q->ccp);

	/* Concatenate the modulus and the operands. Both the modulus and
	 * the operands must be in little endian format.  Since the input
	 * is in big endian format it must be converted and placed in a
	 * fixed length buffer.
	 */
	ret = ccp_init_dm_workarea(&src, cmd_q, CCP_ECC_SRC_BUF_SIZE,
				   DMA_TO_DEVICE);
	if (ret)
		return ret;

	/* Save the workarea address since it is updated in order to perform
	 * the concatenation
	 */
	save = src.address;

	/* Copy the ECC modulus */
	ret = ccp_reverse_set_dm_area(&src, 0, ecc->mod, 0, ecc->mod_len);
	if (ret)
		goto e_src;
	src.address += CCP_ECC_OPERAND_SIZE;

	/* Copy the first operand */
	ret = ccp_reverse_set_dm_area(&src, 0, ecc->u.mm.operand_1, 0,
				      ecc->u.mm.operand_1_len);
	if (ret)
		goto e_src;
	src.address += CCP_ECC_OPERAND_SIZE;

	if (ecc->function != CCP_ECC_FUNCTION_MINV_384BIT) {
		/* Copy the second operand */
		ret = ccp_reverse_set_dm_area(&src, 0, ecc->u.mm.operand_2, 0,
					      ecc->u.mm.operand_2_len);
		if (ret)
			goto e_src;
		src.address += CCP_ECC_OPERAND_SIZE;
	}

	/* Restore the workarea address */
	src.address = save;

	/* Prepare the output area for the operation */
	ret = ccp_init_dm_workarea(&dst, cmd_q, CCP_ECC_DST_BUF_SIZE,
				   DMA_FROM_DEVICE);
	if (ret)
		goto e_src;

	op.soc = 1;
	op.src.u.dma.address = src.dma.address;
	op.src.u.dma.offset = 0;
	op.src.u.dma.length = src.length;
	op.dst.u.dma.address = dst.dma.address;
	op.dst.u.dma.offset = 0;
	op.dst.u.dma.length = dst.length;

	op.u.ecc.function = cmd->u.ecc.function;

	ret = cmd_q->ccp->vdata->perform->ecc(&op);
	if (ret) {
		cmd->engine_error = cmd_q->cmd_error;
		goto e_dst;
	}

	ecc->ecc_result = le16_to_cpup(
		(const __le16 *)(dst.address + CCP_ECC_RESULT_OFFSET));
	if (!(ecc->ecc_result & CCP_ECC_RESULT_SUCCESS)) {
		ret = -EIO;
		goto e_dst;
	}

	/* Save the ECC result */
	ccp_reverse_get_dm_area(&dst, 0, ecc->u.mm.result, 0,
				CCP_ECC_MODULUS_BYTES);

e_dst:
	ccp_dm_free(&dst);

e_src:
	ccp_dm_free(&src);

	return ret;
}