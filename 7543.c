ccp_run_aes_cmd(struct ccp_cmd_queue *cmd_q, struct ccp_cmd *cmd)
{
	struct ccp_aes_engine *aes = &cmd->u.aes;
	struct ccp_dm_workarea key, ctx;
	struct ccp_data src, dst;
	struct ccp_op op;
	unsigned int dm_offset;
	bool in_place = false;
	int ret;

	if (!((aes->key_len == AES_KEYSIZE_128) ||
	      (aes->key_len == AES_KEYSIZE_192) ||
	      (aes->key_len == AES_KEYSIZE_256)))
		return -EINVAL;

	if (((aes->mode == CCP_AES_MODE_ECB) ||
	     (aes->mode == CCP_AES_MODE_CBC)) &&
	    (aes->src_len & (AES_BLOCK_SIZE - 1)))
		return -EINVAL;

	if (!aes->key || !aes->src || !aes->dst)
		return -EINVAL;

	if (aes->mode != CCP_AES_MODE_ECB) {
		if (aes->iv_len != AES_BLOCK_SIZE)
			return -EINVAL;

		if (!aes->iv)
			return -EINVAL;
	}

	BUILD_BUG_ON(CCP_AES_KEY_SB_COUNT != 1);
	BUILD_BUG_ON(CCP_AES_CTX_SB_COUNT != 1);

	ret = -EIO;
	memset(&op, 0, sizeof(op));
	op.cmd_q = cmd_q;
	op.jobid = CCP_NEW_JOBID(cmd_q->ccp);
	op.sb_key = cmd_q->sb_key;
	op.sb_ctx = cmd_q->sb_ctx;
	op.init = (aes->mode == CCP_AES_MODE_ECB) ? 0 : 1;
	op.u.aes.type = aes->type;
	op.u.aes.mode = aes->mode;
	op.u.aes.action = aes->action;

	/* All supported key sizes fit in a single (32-byte) SB entry
	 * and must be in little endian format. Use the 256-bit byte
	 * swap passthru option to convert from big endian to little
	 * endian.
	 */
	ret = ccp_init_dm_workarea(&key, cmd_q,
				   CCP_AES_KEY_SB_COUNT * CCP_SB_BYTES,
				   DMA_TO_DEVICE);
	if (ret)
		return ret;

	dm_offset = CCP_SB_BYTES - aes->key_len;
	ret = ccp_set_dm_area(&key, dm_offset, aes->key, 0, aes->key_len);
	if (ret)
		goto e_key;
	ret = ccp_copy_to_sb(cmd_q, &key, op.jobid, op.sb_key,
			     CCP_PASSTHRU_BYTESWAP_256BIT);
	if (ret) {
		cmd->engine_error = cmd_q->cmd_error;
		goto e_key;
	}

	/* The AES context fits in a single (32-byte) SB entry and
	 * must be in little endian format. Use the 256-bit byte swap
	 * passthru option to convert from big endian to little endian.
	 */
	ret = ccp_init_dm_workarea(&ctx, cmd_q,
				   CCP_AES_CTX_SB_COUNT * CCP_SB_BYTES,
				   DMA_BIDIRECTIONAL);
	if (ret)
		goto e_key;

	if (aes->mode != CCP_AES_MODE_ECB) {
		/* Load the AES context - convert to LE */
		dm_offset = CCP_SB_BYTES - AES_BLOCK_SIZE;
		ret = ccp_set_dm_area(&ctx, dm_offset, aes->iv, 0, aes->iv_len);
		if (ret)
			goto e_ctx;
		ret = ccp_copy_to_sb(cmd_q, &ctx, op.jobid, op.sb_ctx,
				     CCP_PASSTHRU_BYTESWAP_256BIT);
		if (ret) {
			cmd->engine_error = cmd_q->cmd_error;
			goto e_ctx;
		}
	}
	switch (aes->mode) {
	case CCP_AES_MODE_CFB: /* CFB128 only */
	case CCP_AES_MODE_CTR:
		op.u.aes.size = AES_BLOCK_SIZE * BITS_PER_BYTE - 1;
		break;
	default:
		op.u.aes.size = 0;
	}

	/* Prepare the input and output data workareas. For in-place
	 * operations we need to set the dma direction to BIDIRECTIONAL
	 * and copy the src workarea to the dst workarea.
	 */
	if (sg_virt(aes->src) == sg_virt(aes->dst))
		in_place = true;

	ret = ccp_init_data(&src, cmd_q, aes->src, aes->src_len,
			    AES_BLOCK_SIZE,
			    in_place ? DMA_BIDIRECTIONAL : DMA_TO_DEVICE);
	if (ret)
		goto e_ctx;

	if (in_place) {
		dst = src;
	} else {
		ret = ccp_init_data(&dst, cmd_q, aes->dst, aes->src_len,
				    AES_BLOCK_SIZE, DMA_FROM_DEVICE);
		if (ret)
			goto e_src;
	}

	/* Send data to the CCP AES engine */
	while (src.sg_wa.bytes_left) {
		ccp_prepare_data(&src, &dst, &op, AES_BLOCK_SIZE, true);
		if (!src.sg_wa.bytes_left) {
			op.eom = 1;

			/* Since we don't retrieve the AES context in ECB
			 * mode we have to wait for the operation to complete
			 * on the last piece of data
			 */
			if (aes->mode == CCP_AES_MODE_ECB)
				op.soc = 1;
		}

		ret = cmd_q->ccp->vdata->perform->aes(&op);
		if (ret) {
			cmd->engine_error = cmd_q->cmd_error;
			goto e_dst;
		}

		ccp_process_data(&src, &dst, &op);
	}

	if (aes->mode != CCP_AES_MODE_ECB) {
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
		ccp_get_dm_area(&ctx, dm_offset, aes->iv, 0, aes->iv_len);
	}

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