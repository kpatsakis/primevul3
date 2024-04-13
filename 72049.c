static bool __blk_end_bidi_request(struct request *rq, blk_status_t error,
				   unsigned int nr_bytes, unsigned int bidi_bytes)
{
	lockdep_assert_held(rq->q->queue_lock);
	WARN_ON_ONCE(rq->q->mq_ops);

	if (blk_update_bidi_request(rq, error, nr_bytes, bidi_bytes))
		return true;

	blk_finish_request(rq, error);

	return false;
}
