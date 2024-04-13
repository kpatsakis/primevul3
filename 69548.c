void blk_mq_complete_request(struct request *rq)
{
	struct request_queue *q = rq->q;

	if (unlikely(blk_should_fake_timeout(q)))
		return;
	if (!blk_mark_rq_complete(rq))
		__blk_mq_complete_request(rq);
}
