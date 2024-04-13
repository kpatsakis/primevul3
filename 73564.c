xfs_init_bio_from_bh(
	struct bio		*bio,
	struct buffer_head	*bh)
{
	bio->bi_iter.bi_sector = bh->b_blocknr * (bh->b_size >> 9);
	bio->bi_bdev = bh->b_bdev;
}
