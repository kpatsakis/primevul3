xfs_vm_releasepage(
	struct page		*page,
	gfp_t			gfp_mask)
{
	int			delalloc, unwritten;

	trace_xfs_releasepage(page->mapping->host, page, 0, 0);

	/*
	 * mm accommodates an old ext3 case where clean pages might not have had
	 * the dirty bit cleared. Thus, it can send actual dirty pages to
	 * ->releasepage() via shrink_active_list(). Conversely,
	 * block_invalidatepage() can send pages that are still marked dirty
	 * but otherwise have invalidated buffers.
	 *
	 * We've historically freed buffers on the latter. Instead, quietly
	 * filter out all dirty pages to avoid spurious buffer state warnings.
	 * This can likely be removed once shrink_active_list() is fixed.
	 */
	if (PageDirty(page))
		return 0;

	xfs_count_page_state(page, &delalloc, &unwritten);

	if (WARN_ON_ONCE(delalloc))
		return 0;
	if (WARN_ON_ONCE(unwritten))
		return 0;

	return try_to_free_buffers(page);
}
