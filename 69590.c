void blk_mq_requeue_request(struct request *rq)
{
	__blk_mq_requeue_request(rq);

	BUG_ON(blk_queued_rq(rq));
	blk_mq_add_to_requeue_list(rq, true);
}
