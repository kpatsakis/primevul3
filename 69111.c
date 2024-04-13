void drop_inmem_pages(struct inode *inode)
{
	struct f2fs_inode_info *fi = F2FS_I(inode);

	mutex_lock(&fi->inmem_lock);
	__revoke_inmem_pages(inode, &fi->inmem_pages, true, false);
	mutex_unlock(&fi->inmem_lock);

	clear_inode_flag(inode, FI_ATOMIC_FILE);
	clear_inode_flag(inode, FI_HOT_DATA);
	stat_dec_atomic_write(inode);
}
