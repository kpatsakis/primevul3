void write_meta_page(struct f2fs_sb_info *sbi, struct page *page,
					enum iostat_type io_type)
{
	struct f2fs_io_info fio = {
		.sbi = sbi,
		.type = META,
		.op = REQ_OP_WRITE,
		.op_flags = REQ_SYNC | REQ_META | REQ_PRIO,
		.old_blkaddr = page->index,
		.new_blkaddr = page->index,
		.page = page,
		.encrypted_page = NULL,
		.in_list = false,
	};

	if (unlikely(page->index >= MAIN_BLKADDR(sbi)))
		fio.op_flags &= ~REQ_META;

	set_page_writeback(page);
	f2fs_submit_page_write(&fio);

	f2fs_update_iostat(sbi, io_type, F2FS_BLKSIZE);
}
