xfs_vn_symlink(
	struct inode	*dir,
	struct dentry	*dentry,
	const char	*symname)
{
	struct inode	*inode;
	struct xfs_inode *cip = NULL;
	struct xfs_name	name;
	int		error;
	umode_t		mode;

	mode = S_IFLNK |
		(irix_symlink_mode ? 0777 & ~current_umask() : S_IRWXUGO);
	error = xfs_dentry_mode_to_name(&name, dentry, mode);
	if (unlikely(error))
		goto out;

	error = xfs_symlink(XFS_I(dir), &name, symname, mode, &cip);
	if (unlikely(error))
		goto out;

	inode = VFS_I(cip);

	error = xfs_init_security(inode, dir, &dentry->d_name);
	if (unlikely(error))
		goto out_cleanup_inode;

	xfs_setup_iops(cip);

	d_instantiate(dentry, inode);
	xfs_finish_inode_setup(cip);
	return 0;

 out_cleanup_inode:
	xfs_finish_inode_setup(cip);
	xfs_cleanup_inode(dir, inode, dentry);
	xfs_irele(cip);
 out:
	return error;
}
