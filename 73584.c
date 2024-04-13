xfs_writepage_map(
	struct xfs_writepage_ctx *wpc,
	struct writeback_control *wbc,
	struct inode		*inode,
	struct page		*page,
	loff_t			offset,
	__uint64_t              end_offset)
{
	LIST_HEAD(submit_list);
	struct xfs_ioend	*ioend, *next;
	struct buffer_head	*bh, *head;
	ssize_t			len = 1 << inode->i_blkbits;
	int			error = 0;
	int			count = 0;
	int			uptodate = 1;
	unsigned int		new_type;

	bh = head = page_buffers(page);
	offset = page_offset(page);
	do {
		if (offset >= end_offset)
			break;
		if (!buffer_uptodate(bh))
			uptodate = 0;

		/*
		 * set_page_dirty dirties all buffers in a page, independent
		 * of their state.  The dirty state however is entirely
		 * meaningless for holes (!mapped && uptodate), so skip
		 * buffers covering holes here.
		 */
		if (!buffer_mapped(bh) && buffer_uptodate(bh)) {
			wpc->imap_valid = false;
			continue;
		}

		if (buffer_unwritten(bh))
			new_type = XFS_IO_UNWRITTEN;
		else if (buffer_delay(bh))
			new_type = XFS_IO_DELALLOC;
		else if (buffer_uptodate(bh))
			new_type = XFS_IO_OVERWRITE;
		else {
			if (PageUptodate(page))
				ASSERT(buffer_mapped(bh));
			/*
			 * This buffer is not uptodate and will not be
			 * written to disk.  Ensure that we will put any
			 * subsequent writeable buffers into a new
			 * ioend.
			 */
			wpc->imap_valid = false;
			continue;
		}

		if (xfs_is_reflink_inode(XFS_I(inode))) {
			error = xfs_map_cow(wpc, inode, offset, &new_type);
			if (error)
				goto out;
		}

		if (wpc->io_type != new_type) {
			wpc->io_type = new_type;
			wpc->imap_valid = false;
		}

		if (wpc->imap_valid)
			wpc->imap_valid = xfs_imap_valid(inode, &wpc->imap,
							 offset);
		if (!wpc->imap_valid) {
			error = xfs_map_blocks(inode, offset, &wpc->imap,
					     wpc->io_type);
			if (error)
				goto out;
			wpc->imap_valid = xfs_imap_valid(inode, &wpc->imap,
							 offset);
		}
		if (wpc->imap_valid) {
			lock_buffer(bh);
			if (wpc->io_type != XFS_IO_OVERWRITE)
				xfs_map_at_offset(inode, bh, &wpc->imap, offset);
			xfs_add_to_ioend(inode, bh, offset, wpc, wbc, &submit_list);
			count++;
		}

	} while (offset += len, ((bh = bh->b_this_page) != head));

	if (uptodate && bh == head)
		SetPageUptodate(page);

	ASSERT(wpc->ioend || list_empty(&submit_list));

out:
	/*
	 * On error, we have to fail the ioend here because we have locked
	 * buffers in the ioend. If we don't do this, we'll deadlock
	 * invalidating the page as that tries to lock the buffers on the page.
	 * Also, because we may have set pages under writeback, we have to make
	 * sure we run IO completion to mark the error state of the IO
	 * appropriately, so we can't cancel the ioend directly here. That means
	 * we have to mark this page as under writeback if we included any
	 * buffers from it in the ioend chain so that completion treats it
	 * correctly.
	 *
	 * If we didn't include the page in the ioend, the on error we can
	 * simply discard and unlock it as there are no other users of the page
	 * or it's buffers right now. The caller will still need to trigger
	 * submission of outstanding ioends on the writepage context so they are
	 * treated correctly on error.
	 */
	if (count) {
		xfs_start_page_writeback(page, !error);

		/*
		 * Preserve the original error if there was one, otherwise catch
		 * submission errors here and propagate into subsequent ioend
		 * submissions.
		 */
		list_for_each_entry_safe(ioend, next, &submit_list, io_list) {
			int error2;

			list_del_init(&ioend->io_list);
			error2 = xfs_submit_ioend(wbc, ioend, error);
			if (error2 && !error)
				error = error2;
		}
	} else if (error) {
		xfs_aops_discard_page(page);
		ClearPageUptodate(page);
		unlock_page(page);
	} else {
		/*
		 * We can end up here with no error and nothing to write if we
		 * race with a partial page truncate on a sub-page block sized
		 * filesystem. In that case we need to mark the page clean.
		 */
		xfs_start_page_writeback(page, 1);
		end_page_writeback(page);
	}

	mapping_set_error(page->mapping, error);
	return error;
}
