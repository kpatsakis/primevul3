static struct request *get_request(struct request_queue *q, unsigned int op,
		struct bio *bio, blk_mq_req_flags_t flags, gfp_t gfp)
{
	const bool is_sync = op_is_sync(op);
	DEFINE_WAIT(wait);
	struct request_list *rl;
	struct request *rq;

	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	rl = blk_get_rl(q, bio);	/* transferred to @rq on success */
retry:
	rq = __get_request(rl, op, bio, flags, gfp);
	if (!IS_ERR(rq))
		return rq;

	if (op & REQ_NOWAIT) {
		blk_put_rl(rl);
		return ERR_PTR(-EAGAIN);
	}

	if ((flags & BLK_MQ_REQ_NOWAIT) || unlikely(blk_queue_dying(q))) {
		blk_put_rl(rl);
		return rq;
	}

	/* wait on @rl and retry */
	prepare_to_wait_exclusive(&rl->wait[is_sync], &wait,
				  TASK_UNINTERRUPTIBLE);

	trace_block_sleeprq(q, bio, op);

	spin_unlock_irq(q->queue_lock);
	io_schedule();

	/*
	 * After sleeping, we become a "batching" process and will be able
	 * to allocate at least one request, and up to a big batch of them
	 * for a small period time.  See ioc_batching, ioc_set_batching
	 */
	ioc_set_batching(q, current->io_context);

	spin_lock_irq(q->queue_lock);
	finish_wait(&rl->wait[is_sync], &wait);

	goto retry;
}
