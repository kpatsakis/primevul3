struct request *blk_fetch_request(struct request_queue *q)
{
	struct request *rq;

	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	rq = blk_peek_request(q);
	if (rq)
		blk_start_request(rq);
	return rq;
}
