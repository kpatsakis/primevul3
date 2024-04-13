xfs_vn_lookup(
	struct inode	*dir,
	struct dentry	*dentry,
	unsigned int flags)
{
	struct inode *inode;
	struct xfs_inode *cip;
	struct xfs_name	name;
	int		error;

	if (dentry->d_name.len >= MAXNAMELEN)
		return ERR_PTR(-ENAMETOOLONG);

	xfs_dentry_to_name(&name, dentry);
	error = xfs_lookup(XFS_I(dir), &name, &cip, NULL);
	if (likely(!error))
		inode = VFS_I(cip);
	else if (likely(error == -ENOENT))
		inode = NULL;
	else
		inode = ERR_PTR(error);
	return d_splice_alias(inode, dentry);
}
