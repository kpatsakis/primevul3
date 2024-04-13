xfs_chain_bio(
	struct xfs_ioend	*ioend,
	struct writeback_control *wbc,
	struct buffer_head	*bh)
{
	struct bio *new;

	new = bio_alloc(GFP_NOFS, BIO_MAX_PAGES);
	xfs_init_bio_from_bh(new, bh);

	bio_chain(ioend->io_bio, new);
	bio_get(ioend->io_bio);		/* for xfs_destroy_ioend */
	bio_set_op_attrs(ioend->io_bio, REQ_OP_WRITE,
			  (wbc->sync_mode == WB_SYNC_ALL) ? WRITE_SYNC : 0);
	submit_bio(ioend->io_bio);
	ioend->io_bio = new;
}
