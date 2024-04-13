void f2fs_update_extent_cache_range(struct dnode_of_data *dn,
				pgoff_t fofs, block_t blkaddr, unsigned int len)

{
	if (!f2fs_may_extent_tree(dn->inode))
		return;

	f2fs_update_extent_tree_range(dn->inode, fofs, blkaddr, len);
}
