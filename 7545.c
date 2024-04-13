static int ccp_copy_to_from_sb(struct ccp_cmd_queue *cmd_q,
			       struct ccp_dm_workarea *wa, u32 jobid, u32 sb,
			       u32 byte_swap, bool from)
{
	struct ccp_op op;

	memset(&op, 0, sizeof(op));

	op.cmd_q = cmd_q;
	op.jobid = jobid;
	op.eom = 1;

	if (from) {
		op.soc = 1;
		op.src.type = CCP_MEMTYPE_SB;
		op.src.u.sb = sb;
		op.dst.type = CCP_MEMTYPE_SYSTEM;
		op.dst.u.dma.address = wa->dma.address;
		op.dst.u.dma.length = wa->length;
	} else {
		op.src.type = CCP_MEMTYPE_SYSTEM;
		op.src.u.dma.address = wa->dma.address;
		op.src.u.dma.length = wa->length;
		op.dst.type = CCP_MEMTYPE_SB;
		op.dst.u.sb = sb;
	}

	op.u.passthru.byte_swap = byte_swap;

	return cmd_q->ccp->vdata->perform->passthru(&op);
}