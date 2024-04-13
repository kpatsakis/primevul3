static void blk_mq_rq_ctx_init(struct request_queue *q, struct blk_mq_ctx *ctx,
			       struct request *rq, unsigned int rw_flags)
{
	if (blk_queue_io_stat(q))
		rw_flags |= REQ_IO_STAT;

	INIT_LIST_HEAD(&rq->queuelist);
	/* csd/requeue_work/fifo_time is initialized before use */
	rq->q = q;
	rq->mq_ctx = ctx;
	rq->cmd_flags |= rw_flags;
	/* do not touch atomic flags, it needs atomic ops against the timer */
	rq->cpu = -1;
	INIT_HLIST_NODE(&rq->hash);
	RB_CLEAR_NODE(&rq->rb_node);
	rq->rq_disk = NULL;
	rq->part = NULL;
	rq->start_time = jiffies;
#ifdef CONFIG_BLK_CGROUP
	rq->rl = NULL;
	set_start_time_ns(rq);
	rq->io_start_time_ns = 0;
#endif
	rq->nr_phys_segments = 0;
#if defined(CONFIG_BLK_DEV_INTEGRITY)
	rq->nr_integrity_segments = 0;
#endif
	rq->special = NULL;
	/* tag was already set */
	rq->errors = 0;

	rq->cmd = rq->__cmd;

	rq->extra_len = 0;
	rq->sense_len = 0;
	rq->resid_len = 0;
	rq->sense = NULL;

	INIT_LIST_HEAD(&rq->timeout_list);
	rq->timeout = 0;

	rq->end_io = NULL;
	rq->end_io_data = NULL;
	rq->next_rq = NULL;

	ctx->rq_dispatched[rw_is_sync(rw_flags)]++;
}
