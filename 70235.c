xfs_vn_create(
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode,
	bool		flags)
{
	return xfs_vn_mknod(dir, dentry, mode, 0);
}
