static struct page *get_next_nat_page(struct f2fs_sb_info *sbi, nid_t nid)
{
	struct page *src_page;
	struct page *dst_page;
	pgoff_t src_off;
	pgoff_t dst_off;
	void *src_addr;
	void *dst_addr;
	struct f2fs_nm_info *nm_i = NM_I(sbi);

	src_off = current_nat_addr(sbi, nid);
	dst_off = next_nat_addr(sbi, src_off);

	/* get current nat block page with lock */
	src_page = get_meta_page(sbi, src_off);
	dst_page = grab_meta_page(sbi, dst_off);
	f2fs_bug_on(sbi, PageDirty(src_page));

	src_addr = page_address(src_page);
	dst_addr = page_address(dst_page);
	memcpy(dst_addr, src_addr, PAGE_SIZE);
	set_page_dirty(dst_page);
	f2fs_put_page(src_page, 1);

	set_to_next_nat(nm_i, nid);

	return dst_page;
}
