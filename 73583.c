xfs_vm_writepages(
	struct address_space	*mapping,
	struct writeback_control *wbc)
{
	struct xfs_writepage_ctx wpc = {
		.io_type = XFS_IO_INVALID,
	};
	int			ret;

	xfs_iflags_clear(XFS_I(mapping->host), XFS_ITRUNCATED);
	if (dax_mapping(mapping))
		return dax_writeback_mapping_range(mapping,
				xfs_find_bdev_for_inode(mapping->host), wbc);

	ret = write_cache_pages(mapping, wbc, xfs_do_writepage, &wpc);
	if (wpc.ioend)
		ret = xfs_submit_ioend(wbc, wpc.ioend, ret);
	return ret;
}
