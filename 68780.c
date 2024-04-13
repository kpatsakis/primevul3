static struct page *last_fsync_dnode(struct f2fs_sb_info *sbi, nid_t ino)
{
	pgoff_t index, end;
	struct pagevec pvec;
	struct page *last_page = NULL;

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

			if (unlikely(f2fs_cp_error(sbi))) {
				f2fs_put_page(last_page, 0);
				pagevec_release(&pvec);
				return ERR_PTR(-EIO);
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

			if (!PageDirty(page)) {
				/* someone wrote it for us */
				goto continue_unlock;
			}

			if (last_page)
				f2fs_put_page(last_page, 0);

			get_page(page);
			last_page = page;
			unlock_page(page);
		}
		pagevec_release(&pvec);
		cond_resched();
	}
	return last_page;
}
