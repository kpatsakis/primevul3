xfs_vn_tmpfile(
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode)
{
	return xfs_generic_create(dir, dentry, mode, 0, true);
}
