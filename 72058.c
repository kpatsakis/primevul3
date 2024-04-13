static struct request *__get_request(struct request_list *rl, unsigned int op,
		struct bio *bio, blk_mq_req_flags_t flags, gfp_t gfp_mask)
{
	struct request_queue *q = rl->q;
	struct request *rq;
	struct elevator_type *et = q->elevator->type;
	struct io_context *ioc = rq_ioc(bio);
	struct io_cq *icq = NULL;
	const bool is_sync = op_is_sync(op);
	int may_queue;
	req_flags_t rq_flags = RQF_ALLOCED;

	lockdep_assert_held(q->queue_lock);

	if (unlikely(blk_queue_dying(q)))
		return ERR_PTR(-ENODEV);

	may_queue = elv_may_queue(q, op);
	if (may_queue == ELV_MQUEUE_NO)
		goto rq_starved;

	if (rl->count[is_sync]+1 >= queue_congestion_on_threshold(q)) {
		if (rl->count[is_sync]+1 >= q->nr_requests) {
			/*
			 * The queue will fill after this allocation, so set
			 * it as full, and mark this process as "batching".
			 * This process will be allowed to complete a batch of
			 * requests, others will be blocked.
			 */
			if (!blk_rl_full(rl, is_sync)) {
				ioc_set_batching(q, ioc);
				blk_set_rl_full(rl, is_sync);
			} else {
				if (may_queue != ELV_MQUEUE_MUST
						&& !ioc_batching(q, ioc)) {
					/*
					 * The queue is full and the allocating
					 * process is not a "batcher", and not
					 * exempted by the IO scheduler
					 */
					return ERR_PTR(-ENOMEM);
				}
			}
		}
		blk_set_congested(rl, is_sync);
	}

	/*
	 * Only allow batching queuers to allocate up to 50% over the defined
	 * limit of requests, otherwise we could have thousands of requests
	 * allocated with any setting of ->nr_requests
	 */
	if (rl->count[is_sync] >= (3 * q->nr_requests / 2))
		return ERR_PTR(-ENOMEM);

	q->nr_rqs[is_sync]++;
	rl->count[is_sync]++;
	rl->starved[is_sync] = 0;

	/*
	 * Decide whether the new request will be managed by elevator.  If
	 * so, mark @rq_flags and increment elvpriv.  Non-zero elvpriv will
	 * prevent the current elevator from being destroyed until the new
	 * request is freed.  This guarantees icq's won't be destroyed and
	 * makes creating new ones safe.
	 *
	 * Flush requests do not use the elevator so skip initialization.
	 * This allows a request to share the flush and elevator data.
	 *
	 * Also, lookup icq while holding queue_lock.  If it doesn't exist,
	 * it will be created after releasing queue_lock.
	 */
	if (!op_is_flush(op) && !blk_queue_bypass(q)) {
		rq_flags |= RQF_ELVPRIV;
		q->nr_rqs_elvpriv++;
		if (et->icq_cache && ioc)
			icq = ioc_lookup_icq(ioc, q);
	}

	if (blk_queue_io_stat(q))
		rq_flags |= RQF_IO_STAT;
	spin_unlock_irq(q->queue_lock);

	/* allocate and init request */
	rq = mempool_alloc(rl->rq_pool, gfp_mask);
	if (!rq)
		goto fail_alloc;

	blk_rq_init(q, rq);
	blk_rq_set_rl(rq, rl);
	rq->cmd_flags = op;
	rq->rq_flags = rq_flags;
	if (flags & BLK_MQ_REQ_PREEMPT)
		rq->rq_flags |= RQF_PREEMPT;

	/* init elvpriv */
	if (rq_flags & RQF_ELVPRIV) {
		if (unlikely(et->icq_cache && !icq)) {
			if (ioc)
				icq = ioc_create_icq(ioc, q, gfp_mask);
			if (!icq)
				goto fail_elvpriv;
		}

		rq->elv.icq = icq;
		if (unlikely(elv_set_request(q, rq, bio, gfp_mask)))
			goto fail_elvpriv;

		/* @rq->elv.icq holds io_context until @rq is freed */
		if (icq)
			get_io_context(icq->ioc);
	}
out:
	/*
	 * ioc may be NULL here, and ioc_batching will be false. That's
	 * OK, if the queue is under the request limit then requests need
	 * not count toward the nr_batch_requests limit. There will always
	 * be some limit enforced by BLK_BATCH_TIME.
	 */
	if (ioc_batching(q, ioc))
		ioc->nr_batch_requests--;

	trace_block_getrq(q, bio, op);
	return rq;

fail_elvpriv:
	/*
	 * elvpriv init failed.  ioc, icq and elvpriv aren't mempool backed
	 * and may fail indefinitely under memory pressure and thus
	 * shouldn't stall IO.  Treat this request as !elvpriv.  This will
	 * disturb iosched and blkcg but weird is bettern than dead.
	 */
	printk_ratelimited(KERN_WARNING "%s: dev %s: request aux data allocation failed, iosched may be disturbed\n",
			   __func__, dev_name(q->backing_dev_info->dev));

	rq->rq_flags &= ~RQF_ELVPRIV;
	rq->elv.icq = NULL;

	spin_lock_irq(q->queue_lock);
	q->nr_rqs_elvpriv--;
	spin_unlock_irq(q->queue_lock);
	goto out;

fail_alloc:
	/*
	 * Allocation failed presumably due to memory. Undo anything we
	 * might have messed up.
	 *
	 * Allocating task should really be put onto the front of the wait
	 * queue, but this is pretty rare.
	 */
	spin_lock_irq(q->queue_lock);
	freed_request(rl, is_sync, rq_flags);

	/*
	 * in the very unlikely event that allocation failed and no
	 * requests for this direction was pending, mark us starved so that
	 * freeing of a request in the other direction will notice
	 * us. another possible fix would be to split the rq mempool into
	 * READ and WRITE
	 */
rq_starved:
	if (unlikely(rl->count[is_sync] == 0))
		rl->starved[is_sync] = 1;
	return ERR_PTR(-ENOMEM);
}
