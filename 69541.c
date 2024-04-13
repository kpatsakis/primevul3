struct request *blk_mq_alloc_request(struct request_queue *q, int rw, gfp_t gfp,
		bool reserved)
{
	struct blk_mq_ctx *ctx;
	struct blk_mq_hw_ctx *hctx;
	struct request *rq;
	struct blk_mq_alloc_data alloc_data;
	int ret;

	ret = blk_mq_queue_enter(q, gfp);
	if (ret)
		return ERR_PTR(ret);

	ctx = blk_mq_get_ctx(q);
	hctx = q->mq_ops->map_queue(q, ctx->cpu);
	blk_mq_set_alloc_data(&alloc_data, q, gfp & ~__GFP_WAIT,
			reserved, ctx, hctx);

	rq = __blk_mq_alloc_request(&alloc_data, rw);
	if (!rq && (gfp & __GFP_WAIT)) {
		__blk_mq_run_hw_queue(hctx);
		blk_mq_put_ctx(ctx);

		ctx = blk_mq_get_ctx(q);
		hctx = q->mq_ops->map_queue(q, ctx->cpu);
		blk_mq_set_alloc_data(&alloc_data, q, gfp, reserved, ctx,
				hctx);
		rq =  __blk_mq_alloc_request(&alloc_data, rw);
		ctx = alloc_data.ctx;
	}
	blk_mq_put_ctx(ctx);
	if (!rq) {
		blk_mq_queue_exit(q);
		return ERR_PTR(-EWOULDBLOCK);
	}
	return rq;
}
