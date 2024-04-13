xfs_vm_set_page_dirty(
	struct page		*page)
{
	struct address_space	*mapping = page->mapping;
	struct inode		*inode = mapping->host;
	loff_t			end_offset;
	loff_t			offset;
	int			newly_dirty;

	if (unlikely(!mapping))
		return !TestSetPageDirty(page);

	end_offset = i_size_read(inode);
	offset = page_offset(page);

	spin_lock(&mapping->private_lock);
	if (page_has_buffers(page)) {
		struct buffer_head *head = page_buffers(page);
		struct buffer_head *bh = head;

		do {
			if (offset < end_offset)
				set_buffer_dirty(bh);
			bh = bh->b_this_page;
			offset += 1 << inode->i_blkbits;
		} while (bh != head);
	}
	/*
	 * Lock out page->mem_cgroup migration to keep PageDirty
	 * synchronized with per-memcg dirty page counters.
	 */
	lock_page_memcg(page);
	newly_dirty = !TestSetPageDirty(page);
	spin_unlock(&mapping->private_lock);

	if (newly_dirty) {
		/* sigh - __set_page_dirty() is static, so copy it here, too */
		unsigned long flags;

		spin_lock_irqsave(&mapping->tree_lock, flags);
		if (page->mapping) {	/* Race with truncate? */
			WARN_ON_ONCE(!PageUptodate(page));
			account_page_dirtied(page, mapping);
			radix_tree_tag_set(&mapping->page_tree,
					page_index(page), PAGECACHE_TAG_DIRTY);
		}
		spin_unlock_irqrestore(&mapping->tree_lock, flags);
	}
	unlock_page_memcg(page);
	if (newly_dirty)
		__mark_inode_dirty(mapping->host, I_DIRTY_PAGES);
	return newly_dirty;
}
