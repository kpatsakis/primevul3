static void blk_mq_freeze_queue_wait(struct request_queue *q)
{
	wait_event(q->mq_freeze_wq, percpu_ref_is_zero(&q->mq_usage_counter));
}
