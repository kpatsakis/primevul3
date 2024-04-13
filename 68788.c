void recover_inline_xattr(struct inode *inode, struct page *page)
{
	void *src_addr, *dst_addr;
	size_t inline_size;
	struct page *ipage;
	struct f2fs_inode *ri;

	ipage = get_node_page(F2FS_I_SB(inode), inode->i_ino);
	f2fs_bug_on(F2FS_I_SB(inode), IS_ERR(ipage));

	ri = F2FS_INODE(page);
	if (!(ri->i_inline & F2FS_INLINE_XATTR)) {
		clear_inode_flag(inode, FI_INLINE_XATTR);
		goto update_inode;
	}

	dst_addr = inline_xattr_addr(ipage);
	src_addr = inline_xattr_addr(page);
	inline_size = inline_xattr_size(inode);

	f2fs_wait_on_page_writeback(ipage, NODE, true);
	memcpy(dst_addr, src_addr, inline_size);
update_inode:
	update_inode(inode, ipage);
	f2fs_put_page(ipage, 1);
}
