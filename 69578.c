static void blk_mq_insert_requests(struct request_queue *q,
				     struct blk_mq_ctx *ctx,
				     struct list_head *list,
				     int depth,
				     bool from_schedule)

{
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ctx *current_ctx;

	trace_block_unplug(q, depth, !from_schedule);

	current_ctx = blk_mq_get_ctx(q);

	if (!cpu_online(ctx->cpu))
		ctx = current_ctx;
	hctx = q->mq_ops->map_queue(q, ctx->cpu);

	/*
	 * preemption doesn't flush plug list, so it's possible ctx->cpu is
	 * offline now
	 */
	spin_lock(&ctx->lock);
	while (!list_empty(list)) {
		struct request *rq;

		rq = list_first_entry(list, struct request, queuelist);
		list_del_init(&rq->queuelist);
		rq->mq_ctx = ctx;
		__blk_mq_insert_request(hctx, rq, false);
	}
	spin_unlock(&ctx->lock);

	blk_mq_run_hw_queue(hctx, from_schedule);
	blk_mq_put_ctx(current_ctx);
}
