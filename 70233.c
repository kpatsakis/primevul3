xfs_vn_change_ok(
	struct dentry	*dentry,
	struct iattr	*iattr)
{
	struct xfs_mount	*mp = XFS_I(d_inode(dentry))->i_mount;

	if (mp->m_flags & XFS_MOUNT_RDONLY)
		return -EROFS;

	if (XFS_FORCED_SHUTDOWN(mp))
		return -EIO;

	return setattr_prepare(dentry, iattr);
}
