xfs_attr_get(
	struct xfs_inode	*ip,
	const unsigned char	*name,
	unsigned char		*value,
	int			*valuelenp,
	int			flags)
{
	struct xfs_da_args	args;
	uint			lock_mode;
	int			error;

	XFS_STATS_INC(ip->i_mount, xs_attr_get);

	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		return -EIO;

	error = xfs_attr_args_init(&args, ip, name, flags);
	if (error)
		return error;

	args.value = value;
	args.valuelen = *valuelenp;
	/* Entirely possible to look up a name which doesn't exist */
	args.op_flags = XFS_DA_OP_OKNOENT;

	lock_mode = xfs_ilock_attr_map_shared(ip);
	error = xfs_attr_get_ilocked(ip, &args);
	xfs_iunlock(ip, lock_mode);

	*valuelenp = args.valuelen;
	return error == -EEXIST ? 0 : error;
}
