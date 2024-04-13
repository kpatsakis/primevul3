void blk_mq_free_request(struct request *rq)
{
	struct blk_mq_hw_ctx *hctx;
	struct request_queue *q = rq->q;

	hctx = q->mq_ops->map_queue(q, rq->mq_ctx->cpu);
	blk_mq_free_hctx_request(hctx, rq);
}
