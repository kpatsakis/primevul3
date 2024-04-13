void blk_mq_kick_requeue_list(struct request_queue *q)
{
	kblockd_schedule_work(&q->requeue_work);
}
