xfs_vn_setattr(
	struct dentry		*dentry,
	struct iattr		*iattr)
{
	int			error;

	if (iattr->ia_valid & ATTR_SIZE) {
		struct inode		*inode = d_inode(dentry);
		struct xfs_inode	*ip = XFS_I(inode);
		uint			iolock;

		xfs_ilock(ip, XFS_MMAPLOCK_EXCL);
		iolock = XFS_IOLOCK_EXCL | XFS_MMAPLOCK_EXCL;

		error = xfs_break_layouts(inode, &iolock, BREAK_UNMAP);
		if (error) {
			xfs_iunlock(ip, XFS_MMAPLOCK_EXCL);
			return error;
		}

		error = xfs_vn_setattr_size(dentry, iattr);
		xfs_iunlock(ip, XFS_MMAPLOCK_EXCL);
	} else {
		error = xfs_vn_setattr_nonsize(dentry, iattr);
	}

	return error;
}
