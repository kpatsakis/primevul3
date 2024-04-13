void blk_requeue_request(struct request_queue *q, struct request *rq)
{
	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	blk_delete_timer(rq);
	blk_clear_rq_complete(rq);
	trace_block_rq_requeue(q, rq);
	rq_qos_requeue(q, rq);

	if (rq->rq_flags & RQF_QUEUED)
		blk_queue_end_tag(q, rq);

	BUG_ON(blk_queued_rq(rq));

	elv_requeue_request(q, rq);
}
