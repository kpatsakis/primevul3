xfs_vn_setattr_nonsize(
	struct dentry		*dentry,
	struct iattr		*iattr)
{
	struct xfs_inode	*ip = XFS_I(d_inode(dentry));
	int error;

	trace_xfs_setattr(ip);

	error = xfs_vn_change_ok(dentry, iattr);
	if (error)
		return error;
	return xfs_setattr_nonsize(ip, iattr, 0);
}
