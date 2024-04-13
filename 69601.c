void blk_mq_unfreeze_queue(struct request_queue *q)
{
	int freeze_depth;

	freeze_depth = atomic_dec_return(&q->mq_freeze_depth);
	WARN_ON_ONCE(freeze_depth < 0);
	if (!freeze_depth) {
		percpu_ref_reinit(&q->mq_usage_counter);
		wake_up_all(&q->mq_freeze_wq);
	}
}
