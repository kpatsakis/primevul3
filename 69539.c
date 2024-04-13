void blk_mq_abort_requeue_list(struct request_queue *q)
{
	unsigned long flags;
	LIST_HEAD(rq_list);

	spin_lock_irqsave(&q->requeue_lock, flags);
	list_splice_init(&q->requeue_list, &rq_list);
	spin_unlock_irqrestore(&q->requeue_lock, flags);

	while (!list_empty(&rq_list)) {
		struct request *rq;

		rq = list_first_entry(&rq_list, struct request, queuelist);
		list_del_init(&rq->queuelist);
		rq->errors = -EIO;
		blk_mq_end_request(rq, rq->errors);
	}
 }
