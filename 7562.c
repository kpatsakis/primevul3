static void ccp_process_data(struct ccp_data *src, struct ccp_data *dst,
			     struct ccp_op *op)
{
	op->init = 0;

	if (dst) {
		if (op->dst.u.dma.address == dst->dm_wa.dma.address)
			ccp_empty_queue_buf(dst);
		else
			ccp_update_sg_workarea(&dst->sg_wa,
					       op->dst.u.dma.length);
	}
}