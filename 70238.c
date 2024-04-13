xfs_vn_get_link_inline(
	struct dentry		*dentry,
	struct inode		*inode,
	struct delayed_call	*done)
{
	char			*link;

	ASSERT(XFS_I(inode)->i_df.if_flags & XFS_IFINLINE);

	/*
	 * The VFS crashes on a NULL pointer, so return -EFSCORRUPTED if
	 * if_data is junk.
	 */
	link = XFS_I(inode)->i_df.if_u1.if_data;
	if (!link)
		return ERR_PTR(-EFSCORRUPTED);
	return link;
}
