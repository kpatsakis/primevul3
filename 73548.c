xfs_add_to_ioend(
	struct inode		*inode,
	struct buffer_head	*bh,
	xfs_off_t		offset,
	struct xfs_writepage_ctx *wpc,
	struct writeback_control *wbc,
	struct list_head	*iolist)
{
	if (!wpc->ioend || wpc->io_type != wpc->ioend->io_type ||
	    bh->b_blocknr != wpc->last_block + 1 ||
	    offset != wpc->ioend->io_offset + wpc->ioend->io_size) {
		if (wpc->ioend)
			list_add(&wpc->ioend->io_list, iolist);
		wpc->ioend = xfs_alloc_ioend(inode, wpc->io_type, offset, bh);
	}

	/*
	 * If the buffer doesn't fit into the bio we need to allocate a new
	 * one.  This shouldn't happen more than once for a given buffer.
	 */
	while (xfs_bio_add_buffer(wpc->ioend->io_bio, bh) != bh->b_size)
		xfs_chain_bio(wpc->ioend, wbc, bh);

	wpc->ioend->io_size += bh->b_size;
	wpc->last_block = bh->b_blocknr;
	xfs_start_buffer_writeback(bh);
}
