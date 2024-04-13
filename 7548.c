ccp_run_xts_aes_cmd(struct ccp_cmd_queue *cmd_q, struct ccp_cmd *cmd)
{
	struct ccp_xts_aes_engine *xts = &cmd->u.xts;
	struct ccp_dm_workarea key, ctx;
	struct ccp_data src, dst;
	struct ccp_op op;
	unsigned int unit_size, dm_offset;
	bool in_place = false;
	unsigned int sb_count;
	enum ccp_aes_type aestype;
	int ret;

	switch (xts->unit_size) {
	case CCP_XTS_AES_UNIT_SIZE_16:
		unit_size = 16;
		break;
	case CCP_XTS_AES_UNIT_SIZE_512:
		unit_size = 512;
		break;
	case CCP_XTS_AES_UNIT_SIZE_1024:
		unit_size = 1024;
		break;
	case CCP_XTS_AES_UNIT_SIZE_2048:
		unit_size = 2048;
		break;
	case CCP_XTS_AES_UNIT_SIZE_4096:
		unit_size = 4096;
		break;

	default:
		return -EINVAL;
	}

	if (xts->key_len == AES_KEYSIZE_128)
		aestype = CCP_AES_TYPE_128;
	else if (xts->key_len == AES_KEYSIZE_256)
		aestype = CCP_AES_TYPE_256;
	else
		return -EINVAL;

	if (!xts->final && (xts->src_len & (AES_BLOCK_SIZE - 1)))
		return -EINVAL;

	if (xts->iv_len != AES_BLOCK_SIZE)
		return -EINVAL;

	if (!xts->key || !xts->iv || !xts->src || !xts->dst)
		return -EINVAL;

	BUILD_BUG_ON(CCP_XTS_AES_KEY_SB_COUNT != 1);
	BUILD_BUG_ON(CCP_XTS_AES_CTX_SB_COUNT != 1);

	ret = -EIO;
	memset(&op, 0, sizeof(op));
	op.cmd_q = cmd_q;
	op.jobid = CCP_NEW_JOBID(cmd_q->ccp);
	op.sb_key = cmd_q->sb_key;
	op.sb_ctx = cmd_q->sb_ctx;
	op.init = 1;
	op.u.xts.type = aestype;
	op.u.xts.action = xts->action;
	op.u.xts.unit_size = xts->unit_size;

	/* A version 3 device only supports 128-bit keys, which fits into a
	 * single SB entry. A version 5 device uses a 512-bit vector, so two
	 * SB entries.
	 */
	if (cmd_q->ccp->vdata->version == CCP_VERSION(3, 0))
		sb_count = CCP_XTS_AES_KEY_SB_COUNT;
	else
		sb_count = CCP5_XTS_AES_KEY_SB_COUNT;
	ret = ccp_init_dm_workarea(&key, cmd_q,
				   sb_count * CCP_SB_BYTES,
				   DMA_TO_DEVICE);
	if (ret)
		return ret;

	if (cmd_q->ccp->vdata->version == CCP_VERSION(3, 0)) {
		/* All supported key sizes must be in little endian format.
		 * Use the 256-bit byte swap passthru option to convert from
		 * big endian to little endian.
		 */
		dm_offset = CCP_SB_BYTES - AES_KEYSIZE_128;
		ret = ccp_set_dm_area(&key, dm_offset, xts->key, 0, xts->key_len);
		if (ret)
			goto e_key;
		ret = ccp_set_dm_area(&key, 0, xts->key, xts->key_len, xts->key_len);
		if (ret)
			goto e_key;
	} else {
		/* Version 5 CCPs use a 512-bit space for the key: each portion
		 * occupies 256 bits, or one entire slot, and is zero-padded.
		 */
		unsigned int pad;

		dm_offset = CCP_SB_BYTES;
		pad = dm_offset - xts->key_len;
		ret = ccp_set_dm_area(&key, pad, xts->key, 0, xts->key_len);
		if (ret)
			goto e_key;
		ret = ccp_set_dm_area(&key, dm_offset + pad, xts->key,
				      xts->key_len, xts->key_len);
		if (ret)
			goto e_key;
	}
	ret = ccp_copy_to_sb(cmd_q, &key, op.jobid, op.sb_key,
			     CCP_PASSTHRU_BYTESWAP_256BIT);
	if (ret) {
		cmd->engine_error = cmd_q->cmd_error;
		goto e_key;
	}

	/* The AES context fits in a single (32-byte) SB entry and
	 * for XTS is already in little endian format so no byte swapping
	 * is needed.
	 */
	ret = ccp_init_dm_workarea(&ctx, cmd_q,
				   CCP_XTS_AES_CTX_SB_COUNT * CCP_SB_BYTES,
				   DMA_BIDIRECTIONAL);
	if (ret)
		goto e_key;

	ret = ccp_set_dm_area(&ctx, 0, xts->iv, 0, xts->iv_len);
	if (ret)
		goto e_ctx;
	ret = ccp_copy_to_sb(cmd_q, &ctx, op.jobid, op.sb_ctx,
			     CCP_PASSTHRU_BYTESWAP_NOOP);
	if (ret) {
		cmd->engine_error = cmd_q->cmd_error;
		goto e_ctx;
	}

	/* Prepare the input and output data workareas. For in-place
	 * operations we need to set the dma direction to BIDIRECTIONAL
	 * and copy the src workarea to the dst workarea.
	 */
	if (sg_virt(xts->src) == sg_virt(xts->dst))
		in_place = true;

	ret = ccp_init_data(&src, cmd_q, xts->src, xts->src_len,
			    unit_size,
			    in_place ? DMA_BIDIRECTIONAL : DMA_TO_DEVICE);
	if (ret)
		goto e_ctx;

	if (in_place) {
		dst = src;
	} else {
		ret = ccp_init_data(&dst, cmd_q, xts->dst, xts->src_len,
				    unit_size, DMA_FROM_DEVICE);
		if (ret)
			goto e_src;
	}

	/* Send data to the CCP AES engine */
	while (src.sg_wa.bytes_left) {
		ccp_prepare_data(&src, &dst, &op, unit_size, true);
		if (!src.sg_wa.bytes_left)
			op.eom = 1;

		ret = cmd_q->ccp->vdata->perform->xts_aes(&op);
		if (ret) {
			cmd->engine_error = cmd_q->cmd_error;
			goto e_dst;
		}

		ccp_process_data(&src, &dst, &op);
	}

	/* Retrieve the AES context - convert from LE to BE using
	 * 32-byte (256-bit) byteswapping
	 */
	ret = ccp_copy_from_sb(cmd_q, &ctx, op.jobid, op.sb_ctx,
			       CCP_PASSTHRU_BYTESWAP_256BIT);
	if (ret) {
		cmd->engine_error = cmd_q->cmd_error;
		goto e_dst;
	}

	/* ...but we only need AES_BLOCK_SIZE bytes */
	dm_offset = CCP_SB_BYTES - AES_BLOCK_SIZE;
	ccp_get_dm_area(&ctx, dm_offset, xts->iv, 0, xts->iv_len);

e_dst:
	if (!in_place)
		ccp_free_data(&dst, cmd_q);

e_src:
	ccp_free_data(&src, cmd_q);

e_ctx:
	ccp_dm_free(&ctx);

e_key:
	ccp_dm_free(&key);

	return ret;
}