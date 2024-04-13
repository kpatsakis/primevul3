void blk_set_queue_dying(struct request_queue *q)
{
	blk_queue_flag_set(QUEUE_FLAG_DYING, q);

	/*
	 * When queue DYING flag is set, we need to block new req
	 * entering queue, so we call blk_freeze_queue_start() to
	 * prevent I/O from crossing blk_queue_enter().
	 */
	blk_freeze_queue_start(q);

	if (q->mq_ops)
		blk_mq_wake_waiters(q);
	else {
		struct request_list *rl;

		spin_lock_irq(q->queue_lock);
		blk_queue_for_each_rl(rl, q) {
			if (rl->rq_pool) {
				wake_up_all(&rl->wait[BLK_RW_SYNC]);
				wake_up_all(&rl->wait[BLK_RW_ASYNC]);
			}
		}
		spin_unlock_irq(q->queue_lock);
	}

	/* Make blk_queue_enter() reexamine the DYING flag. */
	wake_up_all(&q->mq_freeze_wq);
}
