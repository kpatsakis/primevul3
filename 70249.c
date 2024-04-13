xfs_vn_update_time(
	struct inode		*inode,
	struct timespec64	*now,
	int			flags)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	int			log_flags = XFS_ILOG_TIMESTAMP;
	struct xfs_trans	*tp;
	int			error;

	trace_xfs_update_time(ip);

	if (inode->i_sb->s_flags & SB_LAZYTIME) {
		if (!((flags & S_VERSION) &&
		      inode_maybe_inc_iversion(inode, false)))
			return generic_update_time(inode, now, flags);

		/* Capture the iversion update that just occurred */
		log_flags |= XFS_ILOG_CORE;
	}

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_fsyncts, 0, 0, 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	if (flags & S_CTIME)
		inode->i_ctime = *now;
	if (flags & S_MTIME)
		inode->i_mtime = *now;
	if (flags & S_ATIME)
		inode->i_atime = *now;

	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
	xfs_trans_log_inode(tp, ip, log_flags);
	return xfs_trans_commit(tp);
}
