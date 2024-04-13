xfs_attr_node_removename(xfs_da_args_t *args)
{
	xfs_da_state_t *state;
	xfs_da_state_blk_t *blk;
	xfs_inode_t *dp;
	struct xfs_buf *bp;
	int retval, error, forkoff;

	trace_xfs_attr_node_removename(args);

	/*
	 * Tie a string around our finger to remind us where we are.
	 */
	dp = args->dp;
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = dp->i_mount;

	/*
	 * Search to see if name exists, and get back a pointer to it.
	 */
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error || (retval != -EEXIST)) {
		if (error == 0)
			error = retval;
		goto out;
	}

	/*
	 * If there is an out-of-line value, de-allocate the blocks.
	 * This is done before we remove the attribute so that we don't
	 * overflow the maximum size of a transaction and/or hit a deadlock.
	 */
	blk = &state->path.blk[ state->path.active-1 ];
	ASSERT(blk->bp != NULL);
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	if (args->rmtblkno > 0) {
		/*
		 * Fill in disk block numbers in the state structure
		 * so that we can get the buffers back after we commit
		 * several transactions in the following calls.
		 */
		error = xfs_attr_fillstate(state);
		if (error)
			goto out;

		/*
		 * Mark the attribute as INCOMPLETE, then bunmapi() the
		 * remote value.
		 */
		error = xfs_attr3_leaf_setflag(args);
		if (error)
			goto out;
		error = xfs_attr_rmtval_remove(args);
		if (error)
			goto out;

		/*
		 * Refill the state structure with buffers, the prior calls
		 * released our buffers.
		 */
		error = xfs_attr_refillstate(state);
		if (error)
			goto out;
	}

	/*
	 * Remove the name and update the hashvals in the tree.
	 */
	blk = &state->path.blk[ state->path.active-1 ];
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	retval = xfs_attr3_leaf_remove(blk->bp, args);
	xfs_da3_fixhashpath(state, &state->path);

	/*
	 * Check to see if the tree needs to be collapsed.
	 */
	if (retval && (state->path.active > 1)) {
		xfs_defer_init(args->dfops, args->firstblock);
		error = xfs_da3_join(state);
		if (error)
			goto out_defer_cancel;
		xfs_defer_ijoin(args->dfops, dp);
		error = xfs_defer_finish(&args->trans, args->dfops);
		if (error)
			goto out_defer_cancel;
		/*
		 * Commit the Btree join operation and start a new trans.
		 */
		error = xfs_trans_roll_inode(&args->trans, dp);
		if (error)
			goto out;
	}

	/*
	 * If the result is small enough, push it all into the inode.
	 */
	if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		/*
		 * Have to get rid of the copy of this dabuf in the state.
		 */
		ASSERT(state->path.active == 1);
		ASSERT(state->path.blk[0].bp);
		state->path.blk[0].bp = NULL;

		error = xfs_attr3_leaf_read(args->trans, args->dp, 0, -1, &bp);
		if (error)
			goto out;

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
		} else
			xfs_trans_brelse(args->trans, bp);
	}
	error = 0;

out:
	xfs_da_state_free(state);
	return error;
out_defer_cancel:
	xfs_defer_cancel(args->dfops);
	goto out;
}
