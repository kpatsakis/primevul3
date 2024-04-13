static void blk_mq_requeue_work(struct work_struct *work)
{
	struct request_queue *q =
		container_of(work, struct request_queue, requeue_work);
	LIST_HEAD(rq_list);
	struct request *rq, *next;
	unsigned long flags;

	spin_lock_irqsave(&q->requeue_lock, flags);
	list_splice_init(&q->requeue_list, &rq_list);
	spin_unlock_irqrestore(&q->requeue_lock, flags);

	list_for_each_entry_safe(rq, next, &rq_list, queuelist) {
		if (!(rq->cmd_flags & REQ_SOFTBARRIER))
			continue;

		rq->cmd_flags &= ~REQ_SOFTBARRIER;
		list_del_init(&rq->queuelist);
		blk_mq_insert_request(rq, true, false, false);
	}

	while (!list_empty(&rq_list)) {
		rq = list_entry(rq_list.next, struct request, queuelist);
		list_del_init(&rq->queuelist);
		blk_mq_insert_request(rq, false, false, false);
	}

	/*
	 * Use the start variant of queue running here, so that running
	 * the requeue work will kick stopped queues.
	 */
	blk_mq_start_hw_queues(q);
}
