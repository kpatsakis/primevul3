static int __write_node_page(struct page *page, bool atomic, bool *submitted,
				struct writeback_control *wbc)
{
	struct f2fs_sb_info *sbi = F2FS_P_SB(page);
	nid_t nid;
	struct node_info ni;
	struct f2fs_io_info fio = {
		.sbi = sbi,
		.type = NODE,
		.op = REQ_OP_WRITE,
		.op_flags = wbc_to_write_flags(wbc),
		.page = page,
		.encrypted_page = NULL,
		.submitted = false,
	};

	trace_f2fs_writepage(page, NODE);

	if (unlikely(is_sbi_flag_set(sbi, SBI_POR_DOING)))
		goto redirty_out;
	if (unlikely(f2fs_cp_error(sbi)))
		goto redirty_out;

	/* get old block addr of this node page */
	nid = nid_of_node(page);
	f2fs_bug_on(sbi, page->index != nid);

	if (wbc->for_reclaim) {
		if (!down_read_trylock(&sbi->node_write))
			goto redirty_out;
	} else {
		down_read(&sbi->node_write);
	}

	get_node_info(sbi, nid, &ni);

	/* This page is already truncated */
	if (unlikely(ni.blk_addr == NULL_ADDR)) {
		ClearPageUptodate(page);
		dec_page_count(sbi, F2FS_DIRTY_NODES);
		up_read(&sbi->node_write);
		unlock_page(page);
		return 0;
	}

	if (atomic && !test_opt(sbi, NOBARRIER))
		fio.op_flags |= REQ_PREFLUSH | REQ_FUA;

	set_page_writeback(page);
	fio.old_blkaddr = ni.blk_addr;
	write_node_page(nid, &fio);
	set_node_addr(sbi, &ni, fio.new_blkaddr, is_fsync_dnode(page));
	dec_page_count(sbi, F2FS_DIRTY_NODES);
	up_read(&sbi->node_write);

	if (wbc->for_reclaim) {
		f2fs_submit_merged_bio_cond(sbi, page->mapping->host, 0,
						page->index, NODE, WRITE);
		submitted = NULL;
	}

	unlock_page(page);

	if (unlikely(f2fs_cp_error(sbi))) {
		f2fs_submit_merged_bio(sbi, NODE, WRITE);
		submitted = NULL;
	}
	if (submitted)
		*submitted = fio.submitted;

	return 0;

redirty_out:
	redirty_page_for_writepage(wbc, page);
	return AOP_WRITEPAGE_ACTIVATE;
}
