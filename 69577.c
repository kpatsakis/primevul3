void blk_mq_insert_request(struct request *rq, bool at_head, bool run_queue,
		bool async)
{
	struct request_queue *q = rq->q;
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ctx *ctx = rq->mq_ctx, *current_ctx;

	current_ctx = blk_mq_get_ctx(q);
	if (!cpu_online(ctx->cpu))
		rq->mq_ctx = ctx = current_ctx;

	hctx = q->mq_ops->map_queue(q, ctx->cpu);

	spin_lock(&ctx->lock);
	__blk_mq_insert_request(hctx, rq, at_head);
	spin_unlock(&ctx->lock);

	if (run_queue)
		blk_mq_run_hw_queue(hctx, async);

	blk_mq_put_ctx(current_ctx);
}
