int rewrite_data_page(struct f2fs_io_info *fio)
{
	int err;

	fio->new_blkaddr = fio->old_blkaddr;
	stat_inc_inplace_blocks(fio->sbi);

	err = f2fs_submit_page_bio(fio);

	f2fs_update_iostat(fio->sbi, fio->io_type, F2FS_BLKSIZE);

	return err;
}
