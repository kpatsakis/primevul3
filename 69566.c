static void blk_mq_hctx_clear_pending(struct blk_mq_hw_ctx *hctx,
				      struct blk_mq_ctx *ctx)
{
	struct blk_align_bitmap *bm = get_bm(hctx, ctx);

	clear_bit(CTX_TO_BIT(hctx, ctx), &bm->word);
}
