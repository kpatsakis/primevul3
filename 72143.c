int blk_update_nr_requests(struct request_queue *q, unsigned int nr)
{
	struct request_list *rl;
	int on_thresh, off_thresh;

	WARN_ON_ONCE(q->mq_ops);

	spin_lock_irq(q->queue_lock);
	q->nr_requests = nr;
	blk_queue_congestion_threshold(q);
	on_thresh = queue_congestion_on_threshold(q);
	off_thresh = queue_congestion_off_threshold(q);

	blk_queue_for_each_rl(rl, q) {
		if (rl->count[BLK_RW_SYNC] >= on_thresh)
			blk_set_congested(rl, BLK_RW_SYNC);
		else if (rl->count[BLK_RW_SYNC] < off_thresh)
			blk_clear_congested(rl, BLK_RW_SYNC);

		if (rl->count[BLK_RW_ASYNC] >= on_thresh)
			blk_set_congested(rl, BLK_RW_ASYNC);
		else if (rl->count[BLK_RW_ASYNC] < off_thresh)
			blk_clear_congested(rl, BLK_RW_ASYNC);

		if (rl->count[BLK_RW_SYNC] >= q->nr_requests) {
			blk_set_rl_full(rl, BLK_RW_SYNC);
		} else {
			blk_clear_rl_full(rl, BLK_RW_SYNC);
			wake_up(&rl->wait[BLK_RW_SYNC]);
		}

		if (rl->count[BLK_RW_ASYNC] >= q->nr_requests) {
			blk_set_rl_full(rl, BLK_RW_ASYNC);
		} else {
			blk_clear_rl_full(rl, BLK_RW_ASYNC);
			wake_up(&rl->wait[BLK_RW_ASYNC]);
		}
	}

	spin_unlock_irq(q->queue_lock);
	return 0;
}
