int recover_xattr_data(struct inode *inode, struct page *page, block_t blkaddr)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	nid_t prev_xnid = F2FS_I(inode)->i_xattr_nid;
	nid_t new_xnid = nid_of_node(page);
	struct node_info ni;
	struct page *xpage;

	if (!prev_xnid)
		goto recover_xnid;

	/* 1: invalidate the previous xattr nid */
	get_node_info(sbi, prev_xnid, &ni);
	f2fs_bug_on(sbi, ni.blk_addr == NULL_ADDR);
	invalidate_blocks(sbi, ni.blk_addr);
	dec_valid_node_count(sbi, inode);
	set_node_addr(sbi, &ni, NULL_ADDR, false);

recover_xnid:
	/* 2: update xattr nid in inode */
	remove_free_nid(sbi, new_xnid);
	f2fs_i_xnid_write(inode, new_xnid);
	if (unlikely(!inc_valid_node_count(sbi, inode)))
		f2fs_bug_on(sbi, 1);
	update_inode_page(inode);

	/* 3: update and set xattr node page dirty */
	xpage = grab_cache_page(NODE_MAPPING(sbi), new_xnid);
	if (!xpage)
		return -ENOMEM;

	memcpy(F2FS_NODE(xpage), F2FS_NODE(page), PAGE_SIZE);

	get_node_info(sbi, new_xnid, &ni);
	ni.ino = inode->i_ino;
	set_node_addr(sbi, &ni, NEW_ADDR, false);
	set_page_dirty(xpage);
	f2fs_put_page(xpage, 1);

	return 0;
}
