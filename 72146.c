static struct request *elv_next_request(struct request_queue *q)
{
	struct request *rq;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, NULL);

	WARN_ON_ONCE(q->mq_ops);

	while (1) {
		list_for_each_entry(rq, &q->queue_head, queuelist) {
			if (blk_pm_allow_request(rq))
				return rq;

			if (rq->rq_flags & RQF_SOFTBARRIER)
				break;
		}

		/*
		 * Flush request is running and flush request isn't queueable
		 * in the drive, we can hold the queue till flush request is
		 * finished. Even we don't do this, driver can't dispatch next
		 * requests and will requeue them. And this can improve
		 * throughput too. For example, we have request flush1, write1,
		 * flush 2. flush1 is dispatched, then queue is hold, write1
		 * isn't inserted to queue. After flush1 is finished, flush2
		 * will be dispatched. Since disk cache is already clean,
		 * flush2 will be finished very soon, so looks like flush2 is
		 * folded to flush1.
		 * Since the queue is hold, a flag is set to indicate the queue
		 * should be restarted later. Please see flush_end_io() for
		 * details.
		 */
		if (fq->flush_pending_idx != fq->flush_running_idx &&
				!queue_flush_queueable(q)) {
			fq->flush_queue_delayed = 1;
			return NULL;
		}
		if (unlikely(blk_queue_bypass(q)) ||
		    !q->elevator->type->ops.sq.elevator_dispatch_fn(q, 0))
			return NULL;
	}
}
