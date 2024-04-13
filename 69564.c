void blk_mq_freeze_queue_start(struct request_queue *q)
{
	int freeze_depth;

	freeze_depth = atomic_inc_return(&q->mq_freeze_depth);
	if (freeze_depth == 1) {
		percpu_ref_kill(&q->mq_usage_counter);
		blk_mq_run_hw_queues(q, false);
	}
}
