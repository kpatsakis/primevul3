int blk_set_preempt_only(struct request_queue *q)
{
	return blk_queue_flag_test_and_set(QUEUE_FLAG_PREEMPT_ONLY, q);
}
