xfs_vm_bmap(
	struct address_space	*mapping,
	sector_t		block)
{
	struct inode		*inode = (struct inode *)mapping->host;
	struct xfs_inode	*ip = XFS_I(inode);

	trace_xfs_vm_bmap(XFS_I(inode));
	xfs_ilock(ip, XFS_IOLOCK_SHARED);

	/*
	 * The swap code (ab-)uses ->bmap to get a block mapping and then
	 * bypasseѕ the file system for actual I/O.  We really can't allow
	 * that on reflinks inodes, so we have to skip out here.  And yes,
	 * 0 is the magic code for a bmap error..
	 */
	if (xfs_is_reflink_inode(ip)) {
		xfs_iunlock(ip, XFS_IOLOCK_SHARED);
		return 0;
	}
	filemap_write_and_wait(mapping);
	xfs_iunlock(ip, XFS_IOLOCK_SHARED);
	return generic_block_bmap(mapping, block, xfs_get_blocks);
}
