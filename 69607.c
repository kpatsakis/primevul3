static inline struct blk_align_bitmap *get_bm(struct blk_mq_hw_ctx *hctx,
					      struct blk_mq_ctx *ctx)
{
	return &hctx->ctx_map.map[ctx->index_hw / hctx->ctx_map.bits_per_word];
}
