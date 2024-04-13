ccp_run_passthru_nomap_cmd(struct ccp_cmd_queue *cmd_q,
				      struct ccp_cmd *cmd)
{
	struct ccp_passthru_nomap_engine *pt = &cmd->u.passthru_nomap;
	struct ccp_dm_workarea mask;
	struct ccp_op op;
	int ret;

	if (!pt->final && (pt->src_len & (CCP_PASSTHRU_BLOCKSIZE - 1)))
		return -EINVAL;

	if (!pt->src_dma || !pt->dst_dma)
		return -EINVAL;

	if (pt->bit_mod != CCP_PASSTHRU_BITWISE_NOOP) {
		if (pt->mask_len != CCP_PASSTHRU_MASKSIZE)
			return -EINVAL;
		if (!pt->mask)
			return -EINVAL;
	}

	BUILD_BUG_ON(CCP_PASSTHRU_SB_COUNT != 1);

	memset(&op, 0, sizeof(op));
	op.cmd_q = cmd_q;
	op.jobid = CCP_NEW_JOBID(cmd_q->ccp);

	if (pt->bit_mod != CCP_PASSTHRU_BITWISE_NOOP) {
		/* Load the mask */
		op.sb_key = cmd_q->sb_key;

		mask.length = pt->mask_len;
		mask.dma.address = pt->mask;
		mask.dma.length = pt->mask_len;

		ret = ccp_copy_to_sb(cmd_q, &mask, op.jobid, op.sb_key,
				     CCP_PASSTHRU_BYTESWAP_NOOP);
		if (ret) {
			cmd->engine_error = cmd_q->cmd_error;
			return ret;
		}
	}

	/* Send data to the CCP Passthru engine */
	op.eom = 1;
	op.soc = 1;

	op.src.type = CCP_MEMTYPE_SYSTEM;
	op.src.u.dma.address = pt->src_dma;
	op.src.u.dma.offset = 0;
	op.src.u.dma.length = pt->src_len;

	op.dst.type = CCP_MEMTYPE_SYSTEM;
	op.dst.u.dma.address = pt->dst_dma;
	op.dst.u.dma.offset = 0;
	op.dst.u.dma.length = pt->src_len;

	ret = cmd_q->ccp->vdata->perform->passthru(&op);
	if (ret)
		cmd->engine_error = cmd_q->cmd_error;

	return ret;
}