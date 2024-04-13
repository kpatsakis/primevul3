unsigned int f2fs_destroy_extent_node(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct extent_tree *et = F2FS_I(inode)->extent_tree;
	unsigned int node_cnt = 0;

	if (!et || !atomic_read(&et->node_cnt))
		return 0;

	write_lock(&et->lock);
	node_cnt = __free_extent_tree(sbi, et);
	write_unlock(&et->lock);

	return node_cnt;
}
