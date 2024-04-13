static bool blk_flush_complete_seq(struct request *rq,
				   struct blk_flush_queue *fq,
				   unsigned int seq, int error)
{
	struct request_queue *q = rq->q;
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
	bool queued = false, kicked;

	BUG_ON(rq->flush.seq & seq);
	rq->flush.seq |= seq;

	if (likely(!error))
		seq = blk_flush_cur_seq(rq);
	else
		seq = REQ_FSEQ_DONE;

	switch (seq) {
	case REQ_FSEQ_PREFLUSH:
	case REQ_FSEQ_POSTFLUSH:
		/* queue for flush */
		if (list_empty(pending))
			fq->flush_pending_since = jiffies;
		list_move_tail(&rq->flush.list, pending);
		break;

	case REQ_FSEQ_DATA:
		list_move_tail(&rq->flush.list, &fq->flush_data_in_flight);
		queued = blk_flush_queue_rq(rq, true);
		break;

	case REQ_FSEQ_DONE:
		/*
		 * @rq was previously adjusted by blk_flush_issue() for
		 * flush sequencing and may already have gone through the
		 * flush data request completion path.  Restore @rq for
		 * normal completion and end it.
		 */
		BUG_ON(!list_empty(&rq->queuelist));
		list_del_init(&rq->flush.list);
		blk_flush_restore_request(rq);
		if (q->mq_ops)
			blk_mq_end_request(rq, error);
		else
			__blk_end_request_all(rq, error);
		break;

	default:
		BUG();
	}

	kicked = blk_kick_flush(q, fq);
	return kicked | queued;
}
