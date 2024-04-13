xfs_attr3_leaf_verify(
	struct xfs_buf			*bp)
{
	struct xfs_attr3_icleaf_hdr	ichdr;
	struct xfs_mount		*mp = bp->b_target->bt_mount;
	struct xfs_attr_leafblock	*leaf = bp->b_addr;
	struct xfs_perag		*pag = bp->b_pag;
	struct xfs_attr_leaf_entry	*entries;

	xfs_attr3_leaf_hdr_from_disk(mp->m_attr_geo, &ichdr, leaf);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_node_hdr *hdr3 = bp->b_addr;

		if (ichdr.magic != XFS_ATTR3_LEAF_MAGIC)
			return __this_address;

		if (!uuid_equal(&hdr3->info.uuid, &mp->m_sb.sb_meta_uuid))
			return __this_address;
		if (be64_to_cpu(hdr3->info.blkno) != bp->b_bn)
			return __this_address;
		if (!xfs_log_check_lsn(mp, be64_to_cpu(hdr3->info.lsn)))
			return __this_address;
	} else {
		if (ichdr.magic != XFS_ATTR_LEAF_MAGIC)
			return __this_address;
	}
	/*
	 * In recovery there is a transient state where count == 0 is valid
	 * because we may have transitioned an empty shortform attr to a leaf
	 * if the attr didn't fit in shortform.
	 */
	if (pag && pag->pagf_init && ichdr.count == 0)
		return __this_address;

	/*
	 * firstused is the block offset of the first name info structure.
	 * Make sure it doesn't go off the block or crash into the header.
	 */
	if (ichdr.firstused > mp->m_attr_geo->blksize)
		return __this_address;
	if (ichdr.firstused < xfs_attr3_leaf_hdr_size(leaf))
		return __this_address;

	/* Make sure the entries array doesn't crash into the name info. */
	entries = xfs_attr3_leaf_entryp(bp->b_addr);
	if ((char *)&entries[ichdr.count] >
	    (char *)bp->b_addr + ichdr.firstused)
		return __this_address;

	/* XXX: need to range check rest of attr header values */
	/* XXX: hash order check? */

	return NULL;
}
