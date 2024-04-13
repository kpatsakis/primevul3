static struct request *blk_old_get_request(struct request_queue *q,
				unsigned int op, blk_mq_req_flags_t flags)
{
	struct request *rq;
	gfp_t gfp_mask = flags & BLK_MQ_REQ_NOWAIT ? GFP_ATOMIC : GFP_NOIO;
	int ret = 0;

	WARN_ON_ONCE(q->mq_ops);

	/* create ioc upfront */
	create_io_context(gfp_mask, q->node);

	ret = blk_queue_enter(q, flags);
	if (ret)
		return ERR_PTR(ret);
	spin_lock_irq(q->queue_lock);
	rq = get_request(q, op, NULL, flags, gfp_mask);
	if (IS_ERR(rq)) {
		spin_unlock_irq(q->queue_lock);
		blk_queue_exit(q);
		return rq;
	}

	/* q->queue_lock is unlocked at this point */
	rq->__data_len = 0;
	rq->__sector = (sector_t) -1;
	rq->bio = rq->biotail = NULL;
	return rq;
}
