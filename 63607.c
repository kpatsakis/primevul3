xfs_attr_calc_size(
	struct xfs_da_args	*args,
	int			*local)
{
	struct xfs_mount	*mp = args->dp->i_mount;
	int			size;
	int			nblks;

	/*
	 * Determine space new attribute will use, and if it would be
	 * "local" or "remote" (note: local != inline).
	 */
	size = xfs_attr_leaf_newentsize(args, local);
	nblks = XFS_DAENTER_SPACE_RES(mp, XFS_ATTR_FORK);
	if (*local) {
		if (size > (args->geo->blksize / 2)) {
			/* Double split possible */
			nblks *= 2;
		}
	} else {
		/*
		 * Out of line attribute, cannot double split, but
		 * make room for the attribute value itself.
		 */
		uint	dblocks = xfs_attr3_rmt_blocks(mp, args->valuelen);
		nblks += dblocks;
		nblks += XFS_NEXTENTADD_SPACE_RES(mp, dblocks, XFS_ATTR_FORK);
	}

	return nblks;
}
