static int read_node_page(struct page *page, int op_flags)
{
	struct f2fs_sb_info *sbi = F2FS_P_SB(page);
	struct node_info ni;
	struct f2fs_io_info fio = {
		.sbi = sbi,
		.type = NODE,
		.op = REQ_OP_READ,
		.op_flags = op_flags,
		.page = page,
		.encrypted_page = NULL,
	};

	if (PageUptodate(page))
		return LOCKED_PAGE;

	get_node_info(sbi, page->index, &ni);

	if (unlikely(ni.blk_addr == NULL_ADDR)) {
		ClearPageUptodate(page);
		return -ENOENT;
	}

	fio.new_blkaddr = fio.old_blkaddr = ni.blk_addr;
	return f2fs_submit_page_bio(&fio);
}
