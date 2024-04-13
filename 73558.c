xfs_end_bio(
	struct bio		*bio)
{
	struct xfs_ioend	*ioend = bio->bi_private;
	struct xfs_mount	*mp = XFS_I(ioend->io_inode)->i_mount;

	if (ioend->io_type == XFS_IO_UNWRITTEN || ioend->io_type == XFS_IO_COW)
		queue_work(mp->m_unwritten_workqueue, &ioend->io_work);
	else if (ioend->io_append_trans)
		queue_work(mp->m_data_workqueue, &ioend->io_work);
	else
		xfs_destroy_ioend(ioend, bio->bi_error);
}
