void blk_start_request(struct request *req)
{
	lockdep_assert_held(req->q->queue_lock);
	WARN_ON_ONCE(req->q->mq_ops);

	blk_dequeue_request(req);

	if (test_bit(QUEUE_FLAG_STATS, &req->q->queue_flags)) {
		req->io_start_time_ns = ktime_get_ns();
#ifdef CONFIG_BLK_DEV_THROTTLING_LOW
		req->throtl_size = blk_rq_sectors(req);
#endif
		req->rq_flags |= RQF_STATS;
		rq_qos_issue(req->q, req);
	}

	BUG_ON(blk_rq_is_complete(req));
	blk_add_timer(req);
}
