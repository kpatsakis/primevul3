xfs_vn_link(
	struct dentry	*old_dentry,
	struct inode	*dir,
	struct dentry	*dentry)
{
	struct inode	*inode = d_inode(old_dentry);
	struct xfs_name	name;
	int		error;

	error = xfs_dentry_mode_to_name(&name, dentry, inode->i_mode);
	if (unlikely(error))
		return error;

	error = xfs_link(XFS_I(dir), XFS_I(inode), &name);
	if (unlikely(error))
		return error;

	ihold(inode);
	d_instantiate(dentry, inode);
	return 0;
}
