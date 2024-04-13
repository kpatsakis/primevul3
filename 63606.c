xfs_attr_args_init(
	struct xfs_da_args	*args,
	struct xfs_inode	*dp,
	const unsigned char	*name,
	int			flags)
{

	if (!name)
		return -EINVAL;

	memset(args, 0, sizeof(*args));
	args->geo = dp->i_mount->m_attr_geo;
	args->whichfork = XFS_ATTR_FORK;
	args->dp = dp;
	args->flags = flags;
	args->name = name;
	args->namelen = strlen((const char *)name);
	if (args->namelen >= MAXNAMELEN)
		return -EFAULT;		/* match IRIX behaviour */

	args->hashval = xfs_da_hashname(args->name, args->namelen);
	return 0;
}
