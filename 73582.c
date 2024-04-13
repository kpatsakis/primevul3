xfs_vm_writepage(
	struct page		*page,
	struct writeback_control *wbc)
{
	struct xfs_writepage_ctx wpc = {
		.io_type = XFS_IO_INVALID,
	};
	int			ret;

	ret = xfs_do_writepage(page, wbc, &wpc);
	if (wpc.ioend)
		ret = xfs_submit_ioend(wbc, wpc.ioend, ret);
	return ret;
}
