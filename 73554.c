xfs_check_page_type(
	struct page		*page,
	unsigned int		type,
	bool			check_all_buffers)
{
	struct buffer_head	*bh;
	struct buffer_head	*head;

	if (PageWriteback(page))
		return false;
	if (!page->mapping)
		return false;
	if (!page_has_buffers(page))
		return false;

	bh = head = page_buffers(page);
	do {
		if (buffer_unwritten(bh)) {
			if (type == XFS_IO_UNWRITTEN)
				return true;
		} else if (buffer_delay(bh)) {
			if (type == XFS_IO_DELALLOC)
				return true;
		} else if (buffer_dirty(bh) && buffer_mapped(bh)) {
			if (type == XFS_IO_OVERWRITE)
				return true;
		}

		/* If we are only checking the first buffer, we are done now. */
		if (!check_all_buffers)
			break;
	} while ((bh = bh->b_this_page) != head);

	return false;
}
