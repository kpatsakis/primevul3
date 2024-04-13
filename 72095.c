blk_status_t blk_insert_cloned_request(struct request_queue *q, struct request *rq)
{
	unsigned long flags;
	int where = ELEVATOR_INSERT_BACK;

	if (blk_cloned_rq_check_limits(q, rq))
		return BLK_STS_IOERR;

	if (rq->rq_disk &&
	    should_fail_request(&rq->rq_disk->part0, blk_rq_bytes(rq)))
		return BLK_STS_IOERR;

	if (q->mq_ops) {
		if (blk_queue_io_stat(q))
			blk_account_io_start(rq, true);
		/*
		 * Since we have a scheduler attached on the top device,
		 * bypass a potential scheduler on the bottom device for
		 * insert.
		 */
		return blk_mq_request_issue_directly(rq);
	}

	spin_lock_irqsave(q->queue_lock, flags);
	if (unlikely(blk_queue_dying(q))) {
		spin_unlock_irqrestore(q->queue_lock, flags);
		return BLK_STS_IOERR;
	}

	/*
	 * Submitting request must be dequeued before calling this function
	 * because it will be linked to another request_queue
	 */
	BUG_ON(blk_queued_rq(rq));

	if (op_is_flush(rq->cmd_flags))
		where = ELEVATOR_INSERT_FLUSH;

	add_acct_request(q, rq, where);
	if (where == ELEVATOR_INSERT_FLUSH)
		__blk_run_queue(q);
	spin_unlock_irqrestore(q->queue_lock, flags);

	return BLK_STS_OK;
}
