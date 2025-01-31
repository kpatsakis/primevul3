static void __drop_largest_extent(struct inode *inode,
					pgoff_t fofs, unsigned int len)
{
	struct extent_info *largest = &F2FS_I(inode)->extent_tree->largest;

	if (fofs < largest->fofs + largest->len && fofs + len > largest->fofs) {
		largest->len = 0;
		f2fs_mark_inode_dirty_sync(inode, true);
	}
 }
