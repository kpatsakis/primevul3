void blk_mq_run_hw_queues(struct request_queue *q, bool async)
{
	struct blk_mq_hw_ctx *hctx;
	int i;

	queue_for_each_hw_ctx(q, hctx, i) {
		if ((!blk_mq_hctx_has_pending(hctx) &&
		    list_empty_careful(&hctx->dispatch)) ||
		    test_bit(BLK_MQ_S_STOPPED, &hctx->state))
			continue;

		blk_mq_run_hw_queue(hctx, async);
	}
}
