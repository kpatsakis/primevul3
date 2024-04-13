xfs_vn_rename(
	struct inode	*odir,
	struct dentry	*odentry,
	struct inode	*ndir,
	struct dentry	*ndentry,
	unsigned int	flags)
{
	struct inode	*new_inode = d_inode(ndentry);
	int		omode = 0;
	int		error;
	struct xfs_name	oname;
	struct xfs_name	nname;

	if (flags & ~(RENAME_NOREPLACE | RENAME_EXCHANGE | RENAME_WHITEOUT))
		return -EINVAL;

	/* if we are exchanging files, we need to set i_mode of both files */
	if (flags & RENAME_EXCHANGE)
		omode = d_inode(ndentry)->i_mode;

	error = xfs_dentry_mode_to_name(&oname, odentry, omode);
	if (omode && unlikely(error))
		return error;

	error = xfs_dentry_mode_to_name(&nname, ndentry,
					d_inode(odentry)->i_mode);
	if (unlikely(error))
		return error;

	return xfs_rename(XFS_I(odir), &oname, XFS_I(d_inode(odentry)),
			  XFS_I(ndir), &nname,
			  new_inode ? XFS_I(new_inode) : NULL, flags);
}
