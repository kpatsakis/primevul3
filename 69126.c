void write_node_page(unsigned int nid, struct f2fs_io_info *fio)
{
	struct f2fs_summary sum;

	set_summary(&sum, nid, 0, 0);
	do_write_page(&sum, fio);

	f2fs_update_iostat(fio->sbi, fio->io_type, F2FS_BLKSIZE);
}
