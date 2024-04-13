void blk_sync_queue(struct request_queue *q)
{
	del_timer_sync(&q->timeout);
	cancel_work_sync(&q->timeout_work);

	if (q->mq_ops) {
		struct blk_mq_hw_ctx *hctx;
		int i;

		cancel_delayed_work_sync(&q->requeue_work);
		queue_for_each_hw_ctx(q, hctx, i)
			cancel_delayed_work_sync(&hctx->run_work);
	} else {
		cancel_delayed_work_sync(&q->delay_work);
	}
}
