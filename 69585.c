static inline bool blk_mq_merge_queue_io(struct blk_mq_hw_ctx *hctx,
					 struct blk_mq_ctx *ctx,
					 struct request *rq, struct bio *bio)
{
	if (!hctx_allow_merges(hctx)) {
		blk_mq_bio_to_request(rq, bio);
		spin_lock(&ctx->lock);
insert_rq:
		__blk_mq_insert_request(hctx, rq, false);
		spin_unlock(&ctx->lock);
		return false;
	} else {
		struct request_queue *q = hctx->queue;

		spin_lock(&ctx->lock);
		if (!blk_mq_attempt_merge(q, ctx, bio)) {
			blk_mq_bio_to_request(rq, bio);
			goto insert_rq;
		}

		spin_unlock(&ctx->lock);
		__blk_mq_free_request(hctx, ctx, rq);
		return true;
	}
}
