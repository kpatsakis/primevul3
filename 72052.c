bool __blk_end_request_cur(struct request *rq, blk_status_t error)
{
	return __blk_end_request(rq, error, blk_rq_cur_bytes(rq));
}
