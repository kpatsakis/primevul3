void move_node_page(struct page *node_page, int gc_type)
{
	if (gc_type == FG_GC) {
		struct f2fs_sb_info *sbi = F2FS_P_SB(node_page);
		struct writeback_control wbc = {
			.sync_mode = WB_SYNC_ALL,
			.nr_to_write = 1,
			.for_reclaim = 0,
		};

		set_page_dirty(node_page);
		f2fs_wait_on_page_writeback(node_page, NODE, true);

		f2fs_bug_on(sbi, PageWriteback(node_page));
		if (!clear_page_dirty_for_io(node_page))
			goto out_page;

		if (NODE_MAPPING(sbi)->a_ops->writepage(node_page, &wbc))
			unlock_page(node_page);
		goto release_page;
	} else {
		/* set page dirty and write it */
		if (!PageWriteback(node_page))
			set_page_dirty(node_page);
	}
out_page:
	unlock_page(node_page);
release_page:
	f2fs_put_page(node_page, 0);
}
