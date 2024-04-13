xfs_aops_discard_page(
	struct page		*page)
{
	struct inode		*inode = page->mapping->host;
	struct xfs_inode	*ip = XFS_I(inode);
	struct buffer_head	*bh, *head;
	loff_t			offset = page_offset(page);

	if (!xfs_check_page_type(page, XFS_IO_DELALLOC, true))
		goto out_invalidate;

	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		goto out_invalidate;

	xfs_alert(ip->i_mount,
		"page discard on page %p, inode 0x%llx, offset %llu.",
			page, ip->i_ino, offset);

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	bh = head = page_buffers(page);
	do {
		int		error;
		xfs_fileoff_t	start_fsb;

		if (!buffer_delay(bh))
			goto next_buffer;

		start_fsb = XFS_B_TO_FSBT(ip->i_mount, offset);
		error = xfs_bmap_punch_delalloc_range(ip, start_fsb, 1);
		if (error) {
			/* something screwed, just bail */
			if (!XFS_FORCED_SHUTDOWN(ip->i_mount)) {
				xfs_alert(ip->i_mount,
			"page discard unable to remove delalloc mapping.");
			}
			break;
		}
next_buffer:
		offset += 1 << inode->i_blkbits;

	} while ((bh = bh->b_this_page) != head);

	xfs_iunlock(ip, XFS_ILOCK_EXCL);
out_invalidate:
	xfs_vm_invalidatepage(page, 0, PAGE_SIZE);
	return;
}
