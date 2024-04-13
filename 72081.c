bool blk_end_request(struct request *rq, blk_status_t error,
		unsigned int nr_bytes)
{
	WARN_ON_ONCE(rq->q->mq_ops);
	return blk_end_bidi_request(rq, error, nr_bytes, 0);
}
