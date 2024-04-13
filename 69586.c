static int blk_mq_queue_enter(struct request_queue *q, gfp_t gfp)
{
	while (true) {
		int ret;

		if (percpu_ref_tryget_live(&q->mq_usage_counter))
			return 0;

		if (!(gfp & __GFP_WAIT))
			return -EBUSY;

		ret = wait_event_interruptible(q->mq_freeze_wq,
				!atomic_read(&q->mq_freeze_depth) ||
				blk_queue_dying(q));
		if (blk_queue_dying(q))
			return -ENODEV;
		if (ret)
			return ret;
	}
}
