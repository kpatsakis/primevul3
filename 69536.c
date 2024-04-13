static void __blk_mq_insert_request(struct blk_mq_hw_ctx *hctx,
				    struct request *rq, bool at_head)
{
	struct blk_mq_ctx *ctx = rq->mq_ctx;

	trace_block_rq_insert(hctx->queue, rq);

	if (at_head)
		list_add(&rq->queuelist, &ctx->rq_list);
	else
		list_add_tail(&rq->queuelist, &ctx->rq_list);

	blk_mq_hctx_mark_pending(hctx, ctx);
}
