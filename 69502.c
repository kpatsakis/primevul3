void blk_insert_flush(struct request *rq)
{
	struct request_queue *q = rq->q;
	unsigned int fflags = q->flush_flags;	/* may change, cache */
	unsigned int policy = blk_flush_policy(fflags, rq);
	struct blk_flush_queue *fq = blk_get_flush_queue(q, rq->mq_ctx);

	/*
	 * @policy now records what operations need to be done.  Adjust
	 * REQ_FLUSH and FUA for the driver.
	 */
	rq->cmd_flags &= ~REQ_FLUSH;
	if (!(fflags & REQ_FUA))
		rq->cmd_flags &= ~REQ_FUA;

	/*
	 * An empty flush handed down from a stacking driver may
	 * translate into nothing if the underlying device does not
	 * advertise a write-back cache.  In this case, simply
	 * complete the request.
	 */
	if (!policy) {
		if (q->mq_ops)
			blk_mq_end_request(rq, 0);
		else
			__blk_end_bidi_request(rq, 0, 0, 0);
		return;
	}

	BUG_ON(rq->bio != rq->biotail); /*assumes zero or single bio rq */

	/*
	 * If there's data but flush is not necessary, the request can be
	 * processed directly without going through flush machinery.  Queue
	 * for normal execution.
	 */
	if ((policy & REQ_FSEQ_DATA) &&
	    !(policy & (REQ_FSEQ_PREFLUSH | REQ_FSEQ_POSTFLUSH))) {
		if (q->mq_ops) {
			blk_mq_insert_request(rq, false, false, true);
		} else
			list_add_tail(&rq->queuelist, &q->queue_head);
		return;
	}

	/*
	 * @rq should go through flush machinery.  Mark it part of flush
	 * sequence and submit for further processing.
	 */
	memset(&rq->flush, 0, sizeof(rq->flush));
	INIT_LIST_HEAD(&rq->flush.list);
	rq->cmd_flags |= REQ_FLUSH_SEQ;
	rq->flush.saved_end_io = rq->end_io; /* Usually NULL */
	if (q->mq_ops) {
		rq->end_io = mq_flush_data_end_io;

		spin_lock_irq(&fq->mq_flush_lock);
		blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
		spin_unlock_irq(&fq->mq_flush_lock);
		return;
	}
	rq->end_io = flush_data_end_io;

	blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
}
