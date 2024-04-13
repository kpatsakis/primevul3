static struct page *__get_node_page(struct f2fs_sb_info *sbi, pgoff_t nid,
					struct page *parent, int start)
{
	struct page *page;
	int err;

	if (!nid)
		return ERR_PTR(-ENOENT);
	f2fs_bug_on(sbi, check_nid_range(sbi, nid));
repeat:
	page = f2fs_grab_cache_page(NODE_MAPPING(sbi), nid, false);
	if (!page)
		return ERR_PTR(-ENOMEM);

	err = read_node_page(page, 0);
	if (err < 0) {
		f2fs_put_page(page, 1);
		return ERR_PTR(err);
	} else if (err == LOCKED_PAGE) {
		goto page_hit;
	}

	if (parent)
		ra_node_pages(parent, start + 1, MAX_RA_NODE);

	lock_page(page);

	if (unlikely(page->mapping != NODE_MAPPING(sbi))) {
		f2fs_put_page(page, 1);
		goto repeat;
	}

	if (unlikely(!PageUptodate(page)))
		goto out_err;
page_hit:
	if(unlikely(nid != nid_of_node(page))) {
		f2fs_bug_on(sbi, 1);
		ClearPageUptodate(page);
out_err:
		f2fs_put_page(page, 1);
		return ERR_PTR(-EIO);
	}
	return page;
}
