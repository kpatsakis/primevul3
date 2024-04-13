xfs_map_trim_size(
	struct inode		*inode,
	sector_t		iblock,
	struct buffer_head	*bh_result,
	struct xfs_bmbt_irec	*imap,
	xfs_off_t		offset,
	ssize_t			size)
{
	xfs_off_t		mapping_size;

	mapping_size = imap->br_startoff + imap->br_blockcount - iblock;
	mapping_size <<= inode->i_blkbits;

	ASSERT(mapping_size > 0);
	if (mapping_size > size)
		mapping_size = size;
	if (offset < i_size_read(inode) &&
	    offset + mapping_size >= i_size_read(inode)) {
		/* limit mapping to block that spans EOF */
		mapping_size = roundup_64(i_size_read(inode) - offset,
					  1 << inode->i_blkbits);
	}
	if (mapping_size > LONG_MAX)
		mapping_size = LONG_MAX;

	bh_result->b_size = mapping_size;
}
