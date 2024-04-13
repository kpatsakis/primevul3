static void mq_flush_data_end_io(struct request *rq, int error)
{
	struct request_queue *q = rq->q;
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ctx *ctx = rq->mq_ctx;
	unsigned long flags;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, ctx);

	hctx = q->mq_ops->map_queue(q, ctx->cpu);

	/*
	 * After populating an empty queue, kick it to avoid stall.  Read
	 * the comment in flush_end_io().
	 */
	spin_lock_irqsave(&fq->mq_flush_lock, flags);
	if (blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error))
		blk_mq_run_hw_queue(hctx, true);
	spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
}
