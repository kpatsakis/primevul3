void f2fs_wait_on_block_writeback(struct f2fs_sb_info *sbi, block_t blkaddr)
{
	struct page *cpage;

	if (blkaddr == NEW_ADDR || blkaddr == NULL_ADDR)
		return;

	cpage = find_lock_page(META_MAPPING(sbi), blkaddr);
	if (cpage) {
		f2fs_wait_on_page_writeback(cpage, DATA, true);
		f2fs_put_page(cpage, 1);
	}
}
