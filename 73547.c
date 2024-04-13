__xfs_setfilesize(
	struct xfs_inode	*ip,
	struct xfs_trans	*tp,
	xfs_off_t		offset,
	size_t			size)
{
	xfs_fsize_t		isize;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	isize = xfs_new_eof(ip, offset + size);
	if (!isize) {
		xfs_iunlock(ip, XFS_ILOCK_EXCL);
		xfs_trans_cancel(tp);
		return 0;
	}

	trace_xfs_setfilesize(ip, offset, size);

	ip->i_d.di_size = isize;
	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	return xfs_trans_commit(tp);
}
