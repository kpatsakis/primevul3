xfs_vn_fiemap(
	struct inode		*inode,
	struct fiemap_extent_info *fieinfo,
	u64			start,
	u64			length)
{
	int			error;

	xfs_ilock(XFS_I(inode), XFS_IOLOCK_SHARED);
	if (fieinfo->fi_flags & FIEMAP_FLAG_XATTR) {
		fieinfo->fi_flags &= ~FIEMAP_FLAG_XATTR;
		error = iomap_fiemap(inode, fieinfo, start, length,
				&xfs_xattr_iomap_ops);
	} else {
		error = iomap_fiemap(inode, fieinfo, start, length,
				&xfs_iomap_ops);
	}
	xfs_iunlock(XFS_I(inode), XFS_IOLOCK_SHARED);

	return error;
}
