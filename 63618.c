xfs_attr_set(
	struct xfs_inode	*dp,
	const unsigned char	*name,
	unsigned char		*value,
	int			valuelen,
	int			flags)
{
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_buf		*leaf_bp = NULL;
	struct xfs_da_args	args;
	struct xfs_defer_ops	dfops;
	struct xfs_trans_res	tres;
	xfs_fsblock_t		firstblock;
	int			rsvd = (flags & ATTR_ROOT) != 0;
	int			error, err2, local;

	XFS_STATS_INC(mp, xs_attr_set);

	if (XFS_FORCED_SHUTDOWN(dp->i_mount))
		return -EIO;

	error = xfs_attr_args_init(&args, dp, name, flags);
	if (error)
		return error;

	args.value = value;
	args.valuelen = valuelen;
	args.firstblock = &firstblock;
	args.dfops = &dfops;
	args.op_flags = XFS_DA_OP_ADDNAME | XFS_DA_OP_OKNOENT;
	args.total = xfs_attr_calc_size(&args, &local);

	error = xfs_qm_dqattach(dp, 0);
	if (error)
		return error;

	/*
	 * If the inode doesn't have an attribute fork, add one.
	 * (inode must not be locked when we call this routine)
	 */
	if (XFS_IFORK_Q(dp) == 0) {
		int sf_size = sizeof(xfs_attr_sf_hdr_t) +
			XFS_ATTR_SF_ENTSIZE_BYNAME(args.namelen, valuelen);

		error = xfs_bmap_add_attrfork(dp, sf_size, rsvd);
		if (error)
			return error;
	}

	tres.tr_logres = M_RES(mp)->tr_attrsetm.tr_logres +
			 M_RES(mp)->tr_attrsetrt.tr_logres * args.total;
	tres.tr_logcount = XFS_ATTRSET_LOG_COUNT;
	tres.tr_logflags = XFS_TRANS_PERM_LOG_RES;

	/*
	 * Root fork attributes can use reserved data blocks for this
	 * operation if necessary
	 */
	error = xfs_trans_alloc(mp, &tres, args.total, 0,
			rsvd ? XFS_TRANS_RESERVE : 0, &args.trans);
	if (error)
		return error;

	xfs_ilock(dp, XFS_ILOCK_EXCL);
	error = xfs_trans_reserve_quota_nblks(args.trans, dp, args.total, 0,
				rsvd ? XFS_QMOPT_RES_REGBLKS | XFS_QMOPT_FORCE_RES :
				       XFS_QMOPT_RES_REGBLKS);
	if (error) {
		xfs_iunlock(dp, XFS_ILOCK_EXCL);
		xfs_trans_cancel(args.trans);
		return error;
	}

	xfs_trans_ijoin(args.trans, dp, 0);

	/*
	 * If the attribute list is non-existent or a shortform list,
	 * upgrade it to a single-leaf-block attribute list.
	 */
	if (dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL ||
	    (dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS &&
	     dp->i_d.di_anextents == 0)) {

		/*
		 * Build initial attribute list (if required).
		 */
		if (dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS)
			xfs_attr_shortform_create(&args);

		/*
		 * Try to add the attr to the attribute list in
		 * the inode.
		 */
		error = xfs_attr_shortform_addname(&args);
		if (error != -ENOSPC) {
			/*
			 * Commit the shortform mods, and we're done.
			 * NOTE: this is also the error path (EEXIST, etc).
			 */
			ASSERT(args.trans != NULL);

			/*
			 * If this is a synchronous mount, make sure that
			 * the transaction goes to disk before returning
			 * to the user.
			 */
			if (mp->m_flags & XFS_MOUNT_WSYNC)
				xfs_trans_set_sync(args.trans);

			if (!error && (flags & ATTR_KERNOTIME) == 0) {
				xfs_trans_ichgtime(args.trans, dp,
							XFS_ICHGTIME_CHG);
			}
			err2 = xfs_trans_commit(args.trans);
			xfs_iunlock(dp, XFS_ILOCK_EXCL);

			return error ? error : err2;
		}

		/*
		 * It won't fit in the shortform, transform to a leaf block.
		 * GROT: another possible req'mt for a double-split btree op.
		 */
		xfs_defer_init(args.dfops, args.firstblock);
		error = xfs_attr_shortform_to_leaf(&args, &leaf_bp);
		if (error)
			goto out_defer_cancel;
		/*
		 * Prevent the leaf buffer from being unlocked so that a
		 * concurrent AIL push cannot grab the half-baked leaf
		 * buffer and run into problems with the write verifier.
		 */
		xfs_trans_bhold(args.trans, leaf_bp);
		xfs_defer_bjoin(args.dfops, leaf_bp);
		xfs_defer_ijoin(args.dfops, dp);
		error = xfs_defer_finish(&args.trans, args.dfops);
		if (error)
			goto out_defer_cancel;

		/*
		 * Commit the leaf transformation.  We'll need another (linked)
		 * transaction to add the new attribute to the leaf, which
		 * means that we have to hold & join the leaf buffer here too.
		 */
		error = xfs_trans_roll_inode(&args.trans, dp);
		if (error)
			goto out;
		xfs_trans_bjoin(args.trans, leaf_bp);
		leaf_bp = NULL;
	}

	if (xfs_bmap_one_block(dp, XFS_ATTR_FORK))
		error = xfs_attr_leaf_addname(&args);
	else
		error = xfs_attr_node_addname(&args);
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

out_defer_cancel:
	xfs_defer_cancel(&dfops);
out:
	if (leaf_bp)
		xfs_trans_brelse(args.trans, leaf_bp);
	if (args.trans)
		xfs_trans_cancel(args.trans);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);
	return error;
}
