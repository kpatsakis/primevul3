void blk_mq_release(struct request_queue *q)
{
	struct blk_mq_hw_ctx *hctx;
	unsigned int i;

	/* hctx kobj stays in hctx */
	queue_for_each_hw_ctx(q, hctx, i) {
		if (!hctx)
			continue;
		kfree(hctx->ctxs);
		kfree(hctx);
	}

	kfree(q->queue_hw_ctx);

	/* ctx kobj stays in queue_ctx */
	free_percpu(q->queue_ctx);
}
