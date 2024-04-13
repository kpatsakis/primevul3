static void flush_inline_data(struct f2fs_sb_info *sbi, nid_t ino)
{
	struct inode *inode;
	struct page *page;
	int ret;

	/* should flush inline_data before evict_inode */
	inode = ilookup(sbi->sb, ino);
	if (!inode)
		return;

	page = pagecache_get_page(inode->i_mapping, 0, FGP_LOCK|FGP_NOWAIT, 0);
	if (!page)
		goto iput_out;

	if (!PageUptodate(page))
		goto page_out;

	if (!PageDirty(page))
		goto page_out;

	if (!clear_page_dirty_for_io(page))
		goto page_out;

	ret = f2fs_write_inline_data(inode, page);
	inode_dec_dirty_pages(inode);
	remove_dirty_inode(inode);
	if (ret)
		set_page_dirty(page);
page_out:
	f2fs_put_page(page, 1);
iput_out:
	iput(inode);
}
