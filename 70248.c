xfs_vn_unlink(
	struct inode	*dir,
	struct dentry	*dentry)
{
	struct xfs_name	name;
	int		error;

	xfs_dentry_to_name(&name, dentry);

	error = xfs_remove(XFS_I(dir), &name, XFS_I(d_inode(dentry)));
	if (error)
		return error;

	/*
	 * With unlink, the VFS makes the dentry "negative": no inode,
	 * but still hashed. This is incompatible with case-insensitive
	 * mode, so invalidate (unhash) the dentry in CI-mode.
	 */
	if (xfs_sb_version_hasasciici(&XFS_M(dir->i_sb)->m_sb))
		d_invalidate(dentry);
	return 0;
}
