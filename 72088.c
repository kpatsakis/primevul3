static inline void blk_free_request(struct request_list *rl, struct request *rq)
{
	if (rq->rq_flags & RQF_ELVPRIV) {
		elv_put_request(rl->q, rq);
		if (rq->elv.icq)
			put_io_context(rq->elv.icq->ioc);
	}

	mempool_free(rq, rl->rq_pool);
}
