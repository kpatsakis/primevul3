static bool blk_end_bidi_request(struct request *rq, blk_status_t error,
				 unsigned int nr_bytes, unsigned int bidi_bytes)
{
	struct request_queue *q = rq->q;
	unsigned long flags;

	WARN_ON_ONCE(q->mq_ops);

	if (blk_update_bidi_request(rq, error, nr_bytes, bidi_bytes))
		return true;

	spin_lock_irqsave(q->queue_lock, flags);
	blk_finish_request(rq, error);
	spin_unlock_irqrestore(q->queue_lock, flags);

	return false;
}
