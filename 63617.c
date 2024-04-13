xfs_attr_remove(
	struct xfs_inode	*dp,
	const unsigned char	*name,
	int			flags)
{
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_da_args	args;
	struct xfs_defer_ops	dfops;
	xfs_fsblock_t		firstblock;
	int			error;

	XFS_STATS_INC(mp, xs_attr_remove);

	if (XFS_FORCED_SHUTDOWN(dp->i_mount))
		return -EIO;

	error = xfs_attr_args_init(&args, dp, name, flags);
	if (error)
		return error;

	args.firstblock = &firstblock;
	args.dfops = &dfops;

	/*
	 * we have no control over the attribute names that userspace passes us
	 * to remove, so we have to allow the name lookup prior to attribute
	 * removal to fail.
	 */
	args.op_flags = XFS_DA_OP_OKNOENT;

	error = xfs_qm_dqattach(dp, 0);
	if (error)
		return error;

	/*
	 * Root fork attributes can use reserved data blocks for this
	 * operation if necessary
	 */
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_attrrm,
			XFS_ATTRRM_SPACE_RES(mp), 0,
			(flags & ATTR_ROOT) ? XFS_TRANS_RESERVE : 0,
			&args.trans);
	if (error)
		return error;

	xfs_ilock(dp, XFS_ILOCK_EXCL);
	/*
	 * No need to make quota reservations here. We expect to release some
	 * blocks not allocate in the common case.
	 */
	xfs_trans_ijoin(args.trans, dp, 0);

	if (!xfs_inode_hasattr(dp)) {
		error = -ENOATTR;
	} else if (dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL) {
		ASSERT(dp->i_afp->if_flags & XFS_IFINLINE);
		error = xfs_attr_shortform_remove(&args);
	} else if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		error = xfs_attr_leaf_removename(&args);
	} else {
		error = xfs_attr_node_removename(&args);
	}

	if (error)
		goto out;

	/*
	 * If this is a synchronous mount, make sure that the
	 * transaction goes to disk before returning to the user.
	 */
	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(args.trans);

	if ((flags & ATTR_KERNOTIME) == 0)
		xfs_trans_ichgtime(args.trans, dp, XFS_ICHGTIME_CHG);

	/*
	 * Commit the last in the sequence of transactions.
	 */
	xfs_trans_log_inode(args.trans, dp, XFS_ILOG_CORE);
	error = xfs_trans_commit(args.trans);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);

	return error;

out:
	if (args.trans)
		xfs_trans_cancel(args.trans);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);
	return error;
}
