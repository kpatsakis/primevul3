xfs_finish_page_writeback(
	struct inode		*inode,
	struct bio_vec		*bvec,
	int			error)
{
	unsigned int		end = bvec->bv_offset + bvec->bv_len - 1;
	struct buffer_head	*head, *bh, *next;
	unsigned int		off = 0;
	unsigned int		bsize;

	ASSERT(bvec->bv_offset < PAGE_SIZE);
	ASSERT((bvec->bv_offset & ((1 << inode->i_blkbits) - 1)) == 0);
	ASSERT(end < PAGE_SIZE);
	ASSERT((bvec->bv_len & ((1 << inode->i_blkbits) - 1)) == 0);

	bh = head = page_buffers(bvec->bv_page);

	bsize = bh->b_size;
	do {
		next = bh->b_this_page;
		if (off < bvec->bv_offset)
			goto next_bh;
		if (off > end)
			break;
		bh->b_end_io(bh, !error);
next_bh:
		off += bsize;
	} while ((bh = next) != head);
}
