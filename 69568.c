static void blk_mq_hctx_mark_pending(struct blk_mq_hw_ctx *hctx,
				     struct blk_mq_ctx *ctx)
{
	struct blk_align_bitmap *bm = get_bm(hctx, ctx);

	if (!test_bit(CTX_TO_BIT(hctx, ctx), &bm->word))
		set_bit(CTX_TO_BIT(hctx, ctx), &bm->word);
}
