int blk_queue_enter(struct request_queue *q, blk_mq_req_flags_t flags)
{
	const bool preempt = flags & BLK_MQ_REQ_PREEMPT;

	while (true) {
		bool success = false;

		rcu_read_lock();
		if (percpu_ref_tryget_live(&q->q_usage_counter)) {
			/*
			 * The code that sets the PREEMPT_ONLY flag is
			 * responsible for ensuring that that flag is globally
			 * visible before the queue is unfrozen.
			 */
			if (preempt || !blk_queue_preempt_only(q)) {
				success = true;
			} else {
				percpu_ref_put(&q->q_usage_counter);
			}
		}
		rcu_read_unlock();

		if (success)
			return 0;

		if (flags & BLK_MQ_REQ_NOWAIT)
			return -EBUSY;

		/*
		 * read pair of barrier in blk_freeze_queue_start(),
		 * we need to order reading __PERCPU_REF_DEAD flag of
		 * .q_usage_counter and reading .mq_freeze_depth or
		 * queue dying flag, otherwise the following wait may
		 * never return if the two reads are reordered.
		 */
		smp_rmb();

		wait_event(q->mq_freeze_wq,
			   (atomic_read(&q->mq_freeze_depth) == 0 &&
			    (preempt || !blk_queue_preempt_only(q))) ||
			   blk_queue_dying(q));
		if (blk_queue_dying(q))
			return -ENODEV;
	}
}
