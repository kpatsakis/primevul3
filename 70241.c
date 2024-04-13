xfs_vn_mkdir(
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode)
{
	return xfs_vn_mknod(dir, dentry, mode|S_IFDIR, 0);
}
