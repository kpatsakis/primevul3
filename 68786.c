void ra_node_page(struct f2fs_sb_info *sbi, nid_t nid)
{
	struct page *apage;
	int err;

	if (!nid)
		return;
	f2fs_bug_on(sbi, check_nid_range(sbi, nid));

	rcu_read_lock();
	apage = radix_tree_lookup(&NODE_MAPPING(sbi)->page_tree, nid);
	rcu_read_unlock();
	if (apage)
		return;

	apage = f2fs_grab_cache_page(NODE_MAPPING(sbi), nid, false);
	if (!apage)
		return;

	err = read_node_page(apage, REQ_RAHEAD);
	f2fs_put_page(apage, err ? 1 : 0);
}
