void f2fs_drop_extent_tree(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct extent_tree *et = F2FS_I(inode)->extent_tree;

	set_inode_flag(inode, FI_NO_EXTENT);

	write_lock(&et->lock);
	__free_extent_tree(sbi, et);
	__drop_largest_extent(inode, 0, UINT_MAX);
	write_unlock(&et->lock);
}
