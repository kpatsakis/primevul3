void __blk_put_request(struct request_queue *q, struct request *req)
{
	req_flags_t rq_flags = req->rq_flags;

	if (unlikely(!q))
		return;

	if (q->mq_ops) {
		blk_mq_free_request(req);
		return;
	}

	lockdep_assert_held(q->queue_lock);

	blk_req_zone_write_unlock(req);
	blk_pm_put_request(req);

	elv_completed_request(q, req);

	/* this is a bio leak */
	WARN_ON(req->bio != NULL);

	rq_qos_done(q, req);

	/*
	 * Request may not have originated from ll_rw_blk. if not,
	 * it didn't come out of our reserved rq pools
	 */
	if (rq_flags & RQF_ALLOCED) {
		struct request_list *rl = blk_rq_rl(req);
		bool sync = op_is_sync(req->cmd_flags);

		BUG_ON(!list_empty(&req->queuelist));
		BUG_ON(ELV_ON_HASH(req));

		blk_free_request(rl, req);
		freed_request(rl, sync, rq_flags);
		blk_put_rl(rl);
		blk_queue_exit(q);
	}
}
