xfs_start_page_writeback(
	struct page		*page,
	int			clear_dirty)
{
	ASSERT(PageLocked(page));
	ASSERT(!PageWriteback(page));

	/*
	 * if the page was not fully cleaned, we need to ensure that the higher
	 * layers come back to it correctly. That means we need to keep the page
	 * dirty, and for WB_SYNC_ALL writeback we need to ensure the
	 * PAGECACHE_TAG_TOWRITE index mark is not removed so another attempt to
	 * write this page in this writeback sweep will be made.
	 */
	if (clear_dirty) {
		clear_page_dirty_for_io(page);
		set_page_writeback(page);
	} else
		set_page_writeback_keepwrite(page);

	unlock_page(page);
}
