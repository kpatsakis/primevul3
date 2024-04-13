int fsync_node_pages(struct f2fs_sb_info *sbi, struct inode *inode,
			struct writeback_control *wbc, bool atomic)
{
	pgoff_t index, end;
	pgoff_t last_idx = ULONG_MAX;
	struct pagevec pvec;
	int ret = 0;
	struct page *last_page = NULL;
	bool marked = false;
	nid_t ino = inode->i_ino;

	if (atomic) {
		last_page = last_fsync_dnode(sbi, ino);
		if (IS_ERR_OR_NULL(last_page))
			return PTR_ERR_OR_ZERO(last_page);
	}
retry:
	pagevec_init(&pvec, 0);
	index = 0;
	end = ULONG_MAX;

	while (index <= end) {
		int i, nr_pages;
		nr_pages = pagevec_lookup_tag(&pvec, NODE_MAPPING(sbi), &index,
				PAGECACHE_TAG_DIRTY,
				min(end - index, (pgoff_t)PAGEVEC_SIZE-1) + 1);
		if (nr_pages == 0)
			break;

		for (i = 0; i < nr_pages; i++) {
			struct page *page = pvec.pages[i];
			bool submitted = false;

			if (unlikely(f2fs_cp_error(sbi))) {
				f2fs_put_page(last_page, 0);
				pagevec_release(&pvec);
				ret = -EIO;
				goto out;
			}

			if (!IS_DNODE(page) || !is_cold_node(page))
				continue;
			if (ino_of_node(page) != ino)
				continue;

			lock_page(page);

			if (unlikely(page->mapping != NODE_MAPPING(sbi))) {
continue_unlock:
				unlock_page(page);
				continue;
			}
			if (ino_of_node(page) != ino)
				goto continue_unlock;

			if (!PageDirty(page) && page != last_page) {
				/* someone wrote it for us */
				goto continue_unlock;
			}

			f2fs_wait_on_page_writeback(page, NODE, true);
			BUG_ON(PageWriteback(page));

			if (!atomic || page == last_page) {
				set_fsync_mark(page, 1);
				if (IS_INODE(page)) {
					if (is_inode_flag_set(inode,
								FI_DIRTY_INODE))
						update_inode(inode, page);
					set_dentry_mark(page,
						need_dentry_mark(sbi, ino));
				}
				/*  may be written by other thread */
				if (!PageDirty(page))
					set_page_dirty(page);
			}

			if (!clear_page_dirty_for_io(page))
				goto continue_unlock;

			ret = __write_node_page(page, atomic &&
						page == last_page,
						&submitted, wbc);
			if (ret) {
				unlock_page(page);
				f2fs_put_page(last_page, 0);
				break;
			} else if (submitted) {
				last_idx = page->index;
			}

			if (page == last_page) {
				f2fs_put_page(page, 0);
				marked = true;
				break;
			}
		}
		pagevec_release(&pvec);
		cond_resched();

		if (ret || marked)
			break;
	}
	if (!ret && atomic && !marked) {
		f2fs_msg(sbi->sb, KERN_DEBUG,
			"Retry to write fsync mark: ino=%u, idx=%lx",
					ino, last_page->index);
		lock_page(last_page);
		f2fs_wait_on_page_writeback(last_page, NODE, true);
		set_page_dirty(last_page);
		unlock_page(last_page);
		goto retry;
	}
out:
	if (last_idx != ULONG_MAX)
		f2fs_submit_merged_bio_cond(sbi, NULL, ino, last_idx,
							NODE, WRITE);
	return ret ? -EIO: 0;
}
