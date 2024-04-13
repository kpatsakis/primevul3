void blk_unprep_request(struct request *req)
{
	struct request_queue *q = req->q;

	req->rq_flags &= ~RQF_DONTPREP;
	if (q->unprep_rq_fn)
		q->unprep_rq_fn(q, req);
}
