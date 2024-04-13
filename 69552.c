static int blk_mq_direct_issue_request(struct request *rq)
{
	int ret;
	struct request_queue *q = rq->q;
	struct blk_mq_hw_ctx *hctx = q->mq_ops->map_queue(q,
			rq->mq_ctx->cpu);
	struct blk_mq_queue_data bd = {
		.rq = rq,
		.list = NULL,
		.last = 1
	};

	/*
	 * For OK queue, we are done. For error, kill it. Any other
	 * error (busy), just add it to our list as we previously
	 * would have done
	 */
	ret = q->mq_ops->queue_rq(hctx, &bd);
	if (ret == BLK_MQ_RQ_QUEUE_OK)
		return 0;
	else {
		__blk_mq_requeue_request(rq);

		if (ret == BLK_MQ_RQ_QUEUE_ERROR) {
			rq->errors = -EIO;
			blk_mq_end_request(rq, rq->errors);
			return 0;
		}
		return -1;
	}
}
