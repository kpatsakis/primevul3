void blk_clear_preempt_only(struct request_queue *q)
{
	blk_queue_flag_clear(QUEUE_FLAG_PREEMPT_ONLY, q);
	wake_up_all(&q->mq_freeze_wq);
}
