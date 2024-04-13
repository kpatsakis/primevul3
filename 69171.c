void f2fs_destroy_extent_tree(struct inode *inode)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct extent_tree *et = F2FS_I(inode)->extent_tree;
	unsigned int node_cnt = 0;

	if (!et)
		return;

	if (inode->i_nlink && !is_bad_inode(inode) &&
					atomic_read(&et->node_cnt)) {
		mutex_lock(&sbi->extent_tree_lock);
		list_add_tail(&et->list, &sbi->zombie_list);
		atomic_inc(&sbi->total_zombie_tree);
		mutex_unlock(&sbi->extent_tree_lock);
		return;
	}

	/* free all extent info belong to this extent tree */
	node_cnt = f2fs_destroy_extent_node(inode);

	/* delete extent tree entry in radix tree */
	mutex_lock(&sbi->extent_tree_lock);
	f2fs_bug_on(sbi, atomic_read(&et->node_cnt));
	radix_tree_delete(&sbi->extent_tree_root, inode->i_ino);
	kmem_cache_free(extent_tree_slab, et);
	atomic_dec(&sbi->total_ext_tree);
	mutex_unlock(&sbi->extent_tree_lock);

	F2FS_I(inode)->extent_tree = NULL;

	trace_f2fs_destroy_extent_tree(inode, node_cnt);
}
