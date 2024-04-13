xfs_initxattrs(
	struct inode		*inode,
	const struct xattr	*xattr_array,
	void			*fs_info)
{
	const struct xattr	*xattr;
	struct xfs_inode	*ip = XFS_I(inode);
	int			error = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		error = xfs_attr_set(ip, xattr->name, xattr->value,
				      xattr->value_len, ATTR_SECURE);
		if (error < 0)
			break;
	}
	return error;
}
