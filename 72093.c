void blk_init_request_from_bio(struct request *req, struct bio *bio)
{
	struct io_context *ioc = rq_ioc(bio);

	if (bio->bi_opf & REQ_RAHEAD)
		req->cmd_flags |= REQ_FAILFAST_MASK;

	req->__sector = bio->bi_iter.bi_sector;
	if (ioprio_valid(bio_prio(bio)))
		req->ioprio = bio_prio(bio);
	else if (ioc)
		req->ioprio = ioc->ioprio;
	else
		req->ioprio = IOPRIO_PRIO_VALUE(IOPRIO_CLASS_NONE, 0);
	req->write_hint = bio->bi_write_hint;
	blk_rq_bio_prep(req->q, req, bio);
}
