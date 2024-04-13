xfs_setup_iops(
	struct xfs_inode	*ip)
{
	struct inode		*inode = &ip->i_vnode;

	switch (inode->i_mode & S_IFMT) {
	case S_IFREG:
		inode->i_op = &xfs_inode_operations;
		inode->i_fop = &xfs_file_operations;
		if (IS_DAX(inode))
			inode->i_mapping->a_ops = &xfs_dax_aops;
		else
			inode->i_mapping->a_ops = &xfs_address_space_operations;
		break;
	case S_IFDIR:
		if (xfs_sb_version_hasasciici(&XFS_M(inode->i_sb)->m_sb))
			inode->i_op = &xfs_dir_ci_inode_operations;
		else
			inode->i_op = &xfs_dir_inode_operations;
		inode->i_fop = &xfs_dir_file_operations;
		break;
	case S_IFLNK:
		if (ip->i_df.if_flags & XFS_IFINLINE)
			inode->i_op = &xfs_inline_symlink_inode_operations;
		else
			inode->i_op = &xfs_symlink_inode_operations;
		break;
	default:
		inode->i_op = &xfs_inode_operations;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
		break;
	}
}
