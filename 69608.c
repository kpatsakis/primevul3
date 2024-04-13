static inline bool hctx_allow_merges(struct blk_mq_hw_ctx *hctx)
{
	return (hctx->flags & BLK_MQ_F_SHOULD_MERGE) &&
		!blk_queue_nomerges(hctx->queue);
}
