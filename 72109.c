void blk_put_request(struct request *req)
{
	struct request_queue *q = req->q;

	if (q->mq_ops)
		blk_mq_free_request(req);
	else {
		unsigned long flags;

		spin_lock_irqsave(q->queue_lock, flags);
		__blk_put_request(q, req);
		spin_unlock_irqrestore(q->queue_lock, flags);
	}
}
