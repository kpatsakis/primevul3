static bool blk_mq_hctx_has_pending(struct blk_mq_hw_ctx *hctx)
{
	unsigned int i;

	for (i = 0; i < hctx->ctx_map.size; i++)
		if (hctx->ctx_map.map[i].word)
			return true;

	return false;
}
