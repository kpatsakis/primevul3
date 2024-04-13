void blk_flush_plug_list(struct blk_plug *plug, bool from_schedule)
{
	struct request_queue *q;
	struct request *rq;
	LIST_HEAD(list);
	unsigned int depth;

	flush_plug_callbacks(plug, from_schedule);

	if (!list_empty(&plug->mq_list))
		blk_mq_flush_plug_list(plug, from_schedule);

	if (list_empty(&plug->list))
		return;

	list_splice_init(&plug->list, &list);

	list_sort(NULL, &list, plug_rq_cmp);

	q = NULL;
	depth = 0;

	while (!list_empty(&list)) {
		rq = list_entry_rq(list.next);
		list_del_init(&rq->queuelist);
		BUG_ON(!rq->q);
		if (rq->q != q) {
			/*
			 * This drops the queue lock
			 */
			if (q)
				queue_unplugged(q, depth, from_schedule);
			q = rq->q;
			depth = 0;
			spin_lock_irq(q->queue_lock);
		}

		/*
		 * Short-circuit if @q is dead
		 */
		if (unlikely(blk_queue_dying(q))) {
			__blk_end_request_all(rq, BLK_STS_IOERR);
			continue;
		}

		/*
		 * rq is already accounted, so use raw insert
		 */
		if (op_is_flush(rq->cmd_flags))
			__elv_add_request(q, rq, ELEVATOR_INSERT_FLUSH);
		else
			__elv_add_request(q, rq, ELEVATOR_INSERT_SORT_MERGE);

		depth++;
	}

	/*
	 * This drops the queue lock
	 */
	if (q)
		queue_unplugged(q, depth, from_schedule);
}
