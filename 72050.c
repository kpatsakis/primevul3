bool __blk_end_request(struct request *rq, blk_status_t error,
		unsigned int nr_bytes)
{
	lockdep_assert_held(rq->q->queue_lock);
	WARN_ON_ONCE(rq->q->mq_ops);

	return __blk_end_bidi_request(rq, error, nr_bytes, 0);
}
