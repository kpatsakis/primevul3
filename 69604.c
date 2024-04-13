void blk_mq_wake_waiters(struct request_queue *q)
{
	struct blk_mq_hw_ctx *hctx;
	unsigned int i;

	queue_for_each_hw_ctx(q, hctx, i)
		if (blk_mq_hw_queue_mapped(hctx))
			blk_mq_tag_wakeup_all(hctx->tags, true);

	/*
	 * If we are called because the queue has now been marked as
	 * dying, we need to ensure that processes currently waiting on
	 * the queue are notified as well.
	 */
	wake_up_all(&q->mq_freeze_wq);
}
