static void blk_mq_bio_to_request(struct request *rq, struct bio *bio)
{
	init_request_from_bio(rq, bio);

	if (blk_do_io_stat(rq))
		blk_account_io_start(rq, 1);
}
