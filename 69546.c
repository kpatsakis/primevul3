void blk_mq_cancel_requeue_work(struct request_queue *q)
{
	cancel_work_sync(&q->requeue_work);
}
