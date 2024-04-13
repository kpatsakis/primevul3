void blk_finish_request(struct request *req, blk_status_t error)
{
	struct request_queue *q = req->q;
	u64 now = ktime_get_ns();

	lockdep_assert_held(req->q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	if (req->rq_flags & RQF_STATS)
		blk_stat_add(req, now);

	if (req->rq_flags & RQF_QUEUED)
		blk_queue_end_tag(q, req);

	BUG_ON(blk_queued_rq(req));

	if (unlikely(laptop_mode) && !blk_rq_is_passthrough(req))
		laptop_io_completion(req->q->backing_dev_info);

	blk_delete_timer(req);

	if (req->rq_flags & RQF_DONTPREP)
		blk_unprep_request(req);

	blk_account_io_done(req, now);

	if (req->end_io) {
		rq_qos_done(q, req);
		req->end_io(req, error);
	} else {
		if (blk_bidi_rq(req))
			__blk_put_request(req->next_rq->q, req->next_rq);

		__blk_put_request(q, req);
	}
}
