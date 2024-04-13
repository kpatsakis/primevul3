void blk_mq_free_hctx_request(struct blk_mq_hw_ctx *hctx, struct request *rq)
{
	struct blk_mq_ctx *ctx = rq->mq_ctx;

	ctx->rq_completed[rq_is_sync(rq)]++;
	__blk_mq_free_request(hctx, ctx, rq);

}
