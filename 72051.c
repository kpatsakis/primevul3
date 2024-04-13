void __blk_end_request_all(struct request *rq, blk_status_t error)
{
	bool pending;
	unsigned int bidi_bytes = 0;

	lockdep_assert_held(rq->q->queue_lock);
	WARN_ON_ONCE(rq->q->mq_ops);

	if (unlikely(blk_bidi_rq(rq)))
		bidi_bytes = blk_rq_bytes(rq->next_rq);

	pending = __blk_end_bidi_request(rq, error, blk_rq_bytes(rq), bidi_bytes);
	BUG_ON(pending);
}
