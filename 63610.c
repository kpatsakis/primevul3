xfs_attr_get_ilocked(
	struct xfs_inode	*ip,
	struct xfs_da_args	*args)
{
	ASSERT(xfs_isilocked(ip, XFS_ILOCK_SHARED | XFS_ILOCK_EXCL));

	if (!xfs_inode_hasattr(ip))
		return -ENOATTR;
	else if (ip->i_d.di_aformat == XFS_DINODE_FMT_LOCAL)
		return xfs_attr_shortform_getvalue(args);
	else if (xfs_bmap_one_block(ip, XFS_ATTR_FORK))
		return xfs_attr_leaf_get(args);
	else
		return xfs_attr_node_get(args);
}
