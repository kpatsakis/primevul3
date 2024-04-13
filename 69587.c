static void blk_mq_queue_exit(struct request_queue *q)
{
	percpu_ref_put(&q->mq_usage_counter);
}
