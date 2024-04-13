xfs_attr_leaf_removename(xfs_da_args_t *args)
{
	xfs_inode_t *dp;
	struct xfs_buf *bp;
	int error, forkoff;

	trace_xfs_attr_leaf_removename(args);

	/*
	 * Remove the attribute.
	 */
	dp = args->dp;
	args->blkno = 0;
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return error;

	error = xfs_attr3_leaf_lookup_int(bp, args);
	if (error == -ENOATTR) {
		xfs_trans_brelse(args->trans, bp);
		return error;
	}

	xfs_attr3_leaf_remove(bp, args);

	/*
	 * If the result is small enough, shrink it all into the inode.
	 */
	if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
		xfs_defer_init(args->dfops, args->firstblock);
		error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
		/* bp is gone due to xfs_da_shrink_inode */
		if (error)
			goto out_defer_cancel;
		xfs_defer_ijoin(args->dfops, dp);
		error = xfs_defer_finish(&args->trans, args->dfops);
		if (error)
			goto out_defer_cancel;
	}
	return 0;
out_defer_cancel:
	xfs_defer_cancel(args->dfops);
	return error;
}
