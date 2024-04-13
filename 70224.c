xfs_diflags_to_iflags(
	struct inode		*inode,
	struct xfs_inode	*ip)
{
	uint16_t		flags = ip->i_d.di_flags;

	inode->i_flags &= ~(S_IMMUTABLE | S_APPEND | S_SYNC |
			    S_NOATIME | S_DAX);

	if (flags & XFS_DIFLAG_IMMUTABLE)
		inode->i_flags |= S_IMMUTABLE;
	if (flags & XFS_DIFLAG_APPEND)
		inode->i_flags |= S_APPEND;
	if (flags & XFS_DIFLAG_SYNC)
		inode->i_flags |= S_SYNC;
	if (flags & XFS_DIFLAG_NOATIME)
		inode->i_flags |= S_NOATIME;
	if (xfs_inode_supports_dax(ip))
		inode->i_flags |= S_DAX;
}
