static void blk_mq_check_expired(struct blk_mq_hw_ctx *hctx,
		struct request *rq, void *priv, bool reserved)
{
	struct blk_mq_timeout_data *data = priv;

	if (!test_bit(REQ_ATOM_STARTED, &rq->atomic_flags)) {
		/*
		 * If a request wasn't started before the queue was
		 * marked dying, kill it here or it'll go unnoticed.
		 */
		if (unlikely(blk_queue_dying(rq->q))) {
			rq->errors = -EIO;
			blk_mq_complete_request(rq);
		}
		return;
	}
	if (rq->cmd_flags & REQ_NO_TIMEOUT)
		return;

	if (time_after_eq(jiffies, rq->deadline)) {
		if (!blk_mark_rq_complete(rq))
			blk_mq_rq_timed_out(rq, reserved);
	} else if (!data->next_set || time_after(data->next, rq->deadline)) {
		data->next = rq->deadline;
		data->next_set = 1;
	}
}
