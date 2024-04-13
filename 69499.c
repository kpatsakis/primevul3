static bool blk_flush_queue_rq(struct request *rq, bool add_front)
{
	if (rq->q->mq_ops) {
		struct request_queue *q = rq->q;

		blk_mq_add_to_requeue_list(rq, add_front);
		blk_mq_kick_requeue_list(q);
		return false;
	} else {
		if (add_front)
			list_add(&rq->queuelist, &rq->q->queue_head);
		else
			list_add_tail(&rq->queuelist, &rq->q->queue_head);
		return true;
	}
}
