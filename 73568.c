xfs_map_direct(
	struct inode		*inode,
	struct buffer_head	*bh_result,
	struct xfs_bmbt_irec	*imap,
	xfs_off_t		offset,
	bool			is_cow)
{
	uintptr_t		*flags = (uintptr_t *)&bh_result->b_private;
	xfs_off_t		size = bh_result->b_size;

	trace_xfs_get_blocks_map_direct(XFS_I(inode), offset, size,
		ISUNWRITTEN(imap) ? XFS_IO_UNWRITTEN : is_cow ? XFS_IO_COW :
		XFS_IO_OVERWRITE, imap);

	if (ISUNWRITTEN(imap)) {
		*flags |= XFS_DIO_FLAG_UNWRITTEN;
		set_buffer_defer_completion(bh_result);
	} else if (is_cow) {
		*flags |= XFS_DIO_FLAG_COW;
		set_buffer_defer_completion(bh_result);
	}
	if (offset + size > i_size_read(inode) || offset + size < 0) {
		*flags |= XFS_DIO_FLAG_APPEND;
		set_buffer_defer_completion(bh_result);
	}
}
