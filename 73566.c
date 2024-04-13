xfs_map_blocks(
	struct inode		*inode,
	loff_t			offset,
	struct xfs_bmbt_irec	*imap,
	int			type)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	ssize_t			count = 1 << inode->i_blkbits;
	xfs_fileoff_t		offset_fsb, end_fsb;
	int			error = 0;
	int			bmapi_flags = XFS_BMAPI_ENTIRE;
	int			nimaps = 1;

	if (XFS_FORCED_SHUTDOWN(mp))
		return -EIO;

	ASSERT(type != XFS_IO_COW);
	if (type == XFS_IO_UNWRITTEN)
		bmapi_flags |= XFS_BMAPI_IGSTATE;

	xfs_ilock(ip, XFS_ILOCK_SHARED);
	ASSERT(ip->i_d.di_format != XFS_DINODE_FMT_BTREE ||
	       (ip->i_df.if_flags & XFS_IFEXTENTS));
	ASSERT(offset <= mp->m_super->s_maxbytes);

	if (offset + count > mp->m_super->s_maxbytes)
		count = mp->m_super->s_maxbytes - offset;
	end_fsb = XFS_B_TO_FSB(mp, (xfs_ufsize_t)offset + count);
	offset_fsb = XFS_B_TO_FSBT(mp, offset);
	error = xfs_bmapi_read(ip, offset_fsb, end_fsb - offset_fsb,
				imap, &nimaps, bmapi_flags);
	/*
	 * Truncate an overwrite extent if there's a pending CoW
	 * reservation before the end of this extent.  This forces us
	 * to come back to writepage to take care of the CoW.
	 */
	if (nimaps && type == XFS_IO_OVERWRITE)
		xfs_reflink_trim_irec_to_next_cow(ip, offset_fsb, imap);
	xfs_iunlock(ip, XFS_ILOCK_SHARED);

	if (error)
		return error;

	if (type == XFS_IO_DELALLOC &&
	    (!nimaps || isnullstartblock(imap->br_startblock))) {
		error = xfs_iomap_write_allocate(ip, XFS_DATA_FORK, offset,
				imap);
		if (!error)
			trace_xfs_map_blocks_alloc(ip, offset, count, type, imap);
		return error;
	}

#ifdef DEBUG
	if (type == XFS_IO_UNWRITTEN) {
		ASSERT(nimaps);
		ASSERT(imap->br_startblock != HOLESTARTBLOCK);
		ASSERT(imap->br_startblock != DELAYSTARTBLOCK);
	}
#endif
	if (nimaps)
		trace_xfs_map_blocks_found(ip, offset, count, type, imap);
	return 0;
}
