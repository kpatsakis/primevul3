static void clear_node_page_dirty(struct page *page)
{
	struct address_space *mapping = page->mapping;
	unsigned int long flags;

	if (PageDirty(page)) {
		spin_lock_irqsave(&mapping->tree_lock, flags);
		radix_tree_tag_clear(&mapping->page_tree,
				page_index(page),
				PAGECACHE_TAG_DIRTY);
		spin_unlock_irqrestore(&mapping->tree_lock, flags);

		clear_page_dirty_for_io(page);
		dec_page_count(F2FS_M_SB(mapping), F2FS_DIRTY_NODES);
	}
	ClearPageUptodate(page);
}
