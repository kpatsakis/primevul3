int recover_inode_page(struct f2fs_sb_info *sbi, struct page *page)
{
	struct f2fs_inode *src, *dst;
	nid_t ino = ino_of_node(page);
	struct node_info old_ni, new_ni;
	struct page *ipage;

	get_node_info(sbi, ino, &old_ni);

	if (unlikely(old_ni.blk_addr != NULL_ADDR))
		return -EINVAL;
retry:
	ipage = f2fs_grab_cache_page(NODE_MAPPING(sbi), ino, false);
	if (!ipage) {
		congestion_wait(BLK_RW_ASYNC, HZ/50);
		goto retry;
	}

	/* Should not use this inode from free nid list */
	remove_free_nid(sbi, ino);

	if (!PageUptodate(ipage))
		SetPageUptodate(ipage);
	fill_node_footer(ipage, ino, ino, 0, true);

	src = F2FS_INODE(page);
	dst = F2FS_INODE(ipage);

	memcpy(dst, src, (unsigned long)&src->i_ext - (unsigned long)src);
	dst->i_size = 0;
	dst->i_blocks = cpu_to_le64(1);
	dst->i_links = cpu_to_le32(1);
	dst->i_xattr_nid = 0;
	dst->i_inline = src->i_inline & F2FS_INLINE_XATTR;

	new_ni = old_ni;
	new_ni.ino = ino;

	if (unlikely(!inc_valid_node_count(sbi, NULL)))
		WARN_ON(1);
	set_node_addr(sbi, &new_ni, NEW_ADDR, false);
	inc_valid_inode_count(sbi);
	set_page_dirty(ipage);
	f2fs_put_page(ipage, 1);
	return 0;
}
