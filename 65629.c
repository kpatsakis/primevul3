xfs_attr3_leaf_getvalue(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_local *name_loc;
	struct xfs_attr_leaf_name_remote *name_rmt;
	int			valuelen;

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(args->geo, &ichdr, leaf);
	ASSERT(ichdr.count < args->geo->blksize / 8);
	ASSERT(args->index < ichdr.count);

	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];
	if (entry->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr3_leaf_name_local(leaf, args->index);
		ASSERT(name_loc->namelen == args->namelen);
		ASSERT(memcmp(args->name, name_loc->nameval, args->namelen) == 0);
		valuelen = be16_to_cpu(name_loc->valuelen);
		if (args->flags & ATTR_KERNOVAL) {
			args->valuelen = valuelen;
			return 0;
		}
		if (args->valuelen < valuelen) {
			args->valuelen = valuelen;
			return -ERANGE;
		}
		args->valuelen = valuelen;
		memcpy(args->value, &name_loc->nameval[args->namelen], valuelen);
	} else {
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
		ASSERT(name_rmt->namelen == args->namelen);
		ASSERT(memcmp(args->name, name_rmt->name, args->namelen) == 0);
		args->rmtvaluelen = be32_to_cpu(name_rmt->valuelen);
		args->rmtblkno = be32_to_cpu(name_rmt->valueblk);
		args->rmtblkcnt = xfs_attr3_rmt_blocks(args->dp->i_mount,
						       args->rmtvaluelen);
		if (args->flags & ATTR_KERNOVAL) {
			args->valuelen = args->rmtvaluelen;
			return 0;
		}
		if (args->valuelen < args->rmtvaluelen) {
			args->valuelen = args->rmtvaluelen;
			return -ERANGE;
		}
		args->valuelen = args->rmtvaluelen;
	}
	return 0;
}
