static struct request *blk_mq_map_request(struct request_queue *q,
					  struct bio *bio,
					  struct blk_map_ctx *data)
{
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ctx *ctx;
	struct request *rq;
	int rw = bio_data_dir(bio);
	struct blk_mq_alloc_data alloc_data;

	if (unlikely(blk_mq_queue_enter(q, GFP_KERNEL))) {
		bio_io_error(bio);
		return NULL;
	}

	ctx = blk_mq_get_ctx(q);
	hctx = q->mq_ops->map_queue(q, ctx->cpu);

	if (rw_is_sync(bio->bi_rw))
		rw |= REQ_SYNC;

	trace_block_getrq(q, bio, rw);
	blk_mq_set_alloc_data(&alloc_data, q, GFP_ATOMIC, false, ctx,
			hctx);
	rq = __blk_mq_alloc_request(&alloc_data, rw);
	if (unlikely(!rq)) {
		__blk_mq_run_hw_queue(hctx);
		blk_mq_put_ctx(ctx);
		trace_block_sleeprq(q, bio, rw);

		ctx = blk_mq_get_ctx(q);
		hctx = q->mq_ops->map_queue(q, ctx->cpu);
		blk_mq_set_alloc_data(&alloc_data, q,
				__GFP_WAIT|GFP_ATOMIC, false, ctx, hctx);
		rq = __blk_mq_alloc_request(&alloc_data, rw);
		ctx = alloc_data.ctx;
		hctx = alloc_data.hctx;
	}

	hctx->queued++;
	data->hctx = hctx;
	data->ctx = ctx;
	return rq;
}
