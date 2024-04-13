int blkdev_issue_flush(struct block_device *bdev, gfp_t gfp_mask,
		sector_t *error_sector)
{
	struct request_queue *q;
	struct bio *bio;
	int ret = 0;

	if (bdev->bd_disk == NULL)
		return -ENXIO;

	q = bdev_get_queue(bdev);
	if (!q)
		return -ENXIO;

	/*
	 * some block devices may not have their queue correctly set up here
	 * (e.g. loop device without a backing file) and so issuing a flush
	 * here will panic. Ensure there is a request function before issuing
	 * the flush.
	 */
	if (!q->make_request_fn)
		return -ENXIO;

	bio = bio_alloc(gfp_mask, 0);
	bio->bi_bdev = bdev;

	ret = submit_bio_wait(WRITE_FLUSH, bio);

	/*
	 * The driver must store the error location in ->bi_sector, if
	 * it supports it. For non-stacked drivers, this should be
	 * copied from blk_rq_pos(rq).
	 */
	if (error_sector)
		*error_sector = bio->bi_iter.bi_sector;

	bio_put(bio);
	return ret;
}
