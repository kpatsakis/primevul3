xfs_alloc_ioend(
	struct inode		*inode,
	unsigned int		type,
	xfs_off_t		offset,
	struct buffer_head	*bh)
{
	struct xfs_ioend	*ioend;
	struct bio		*bio;

	bio = bio_alloc_bioset(GFP_NOFS, BIO_MAX_PAGES, xfs_ioend_bioset);
	xfs_init_bio_from_bh(bio, bh);

	ioend = container_of(bio, struct xfs_ioend, io_inline_bio);
	INIT_LIST_HEAD(&ioend->io_list);
	ioend->io_type = type;
	ioend->io_inode = inode;
	ioend->io_size = 0;
	ioend->io_offset = offset;
	INIT_WORK(&ioend->io_work, xfs_end_io);
	ioend->io_append_trans = NULL;
	ioend->io_bio = bio;
	return ioend;
}
