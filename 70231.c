xfs_setattr_time(
	struct xfs_inode	*ip,
	struct iattr		*iattr)
{
	struct inode		*inode = VFS_I(ip);

	ASSERT(xfs_isilocked(ip, XFS_ILOCK_EXCL));

	if (iattr->ia_valid & ATTR_ATIME)
		inode->i_atime = iattr->ia_atime;
	if (iattr->ia_valid & ATTR_CTIME)
		inode->i_ctime = iattr->ia_ctime;
	if (iattr->ia_valid & ATTR_MTIME)
		inode->i_mtime = iattr->ia_mtime;
}
