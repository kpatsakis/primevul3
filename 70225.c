xfs_generic_create(
	struct inode	*dir,
	struct dentry	*dentry,
	umode_t		mode,
	dev_t		rdev,
	bool		tmpfile)	/* unnamed file */
{
	struct inode	*inode;
	struct xfs_inode *ip = NULL;
	struct posix_acl *default_acl, *acl;
	struct xfs_name	name;
	int		error;

	/*
	 * Irix uses Missed'em'V split, but doesn't want to see
	 * the upper 5 bits of (14bit) major.
	 */
	if (S_ISCHR(mode) || S_ISBLK(mode)) {
		if (unlikely(!sysv_valid_dev(rdev) || MAJOR(rdev) & ~0x1ff))
			return -EINVAL;
	} else {
		rdev = 0;
	}

	error = posix_acl_create(dir, &mode, &default_acl, &acl);
	if (error)
		return error;

	/* Verify mode is valid also for tmpfile case */
	error = xfs_dentry_mode_to_name(&name, dentry, mode);
	if (unlikely(error))
		goto out_free_acl;

	if (!tmpfile) {
		error = xfs_create(XFS_I(dir), &name, mode, rdev, &ip);
	} else {
		error = xfs_create_tmpfile(XFS_I(dir), mode, &ip);
	}
	if (unlikely(error))
		goto out_free_acl;

	inode = VFS_I(ip);

	error = xfs_init_security(inode, dir, &dentry->d_name);
	if (unlikely(error))
		goto out_cleanup_inode;

#ifdef CONFIG_XFS_POSIX_ACL
	if (default_acl) {
		error = __xfs_set_acl(inode, default_acl, ACL_TYPE_DEFAULT);
		if (error)
			goto out_cleanup_inode;
	}
	if (acl) {
		error = __xfs_set_acl(inode, acl, ACL_TYPE_ACCESS);
		if (error)
			goto out_cleanup_inode;
	}
#endif

	xfs_setup_iops(ip);

	if (tmpfile) {
		/*
		 * The VFS requires that any inode fed to d_tmpfile must have
		 * nlink == 1 so that it can decrement the nlink in d_tmpfile.
		 * However, we created the temp file with nlink == 0 because
		 * we're not allowed to put an inode with nlink > 0 on the
		 * unlinked list.  Therefore we have to set nlink to 1 so that
		 * d_tmpfile can immediately set it back to zero.
		 */
		set_nlink(inode, 1);
		d_tmpfile(dentry, inode);
	} else
		d_instantiate(dentry, inode);

	xfs_finish_inode_setup(ip);

 out_free_acl:
	if (default_acl)
		posix_acl_release(default_acl);
	if (acl)
		posix_acl_release(acl);
	return error;

 out_cleanup_inode:
	xfs_finish_inode_setup(ip);
	if (!tmpfile)
		xfs_cleanup_inode(dir, inode, dentry);
	xfs_irele(ip);
	goto out_free_acl;
}
