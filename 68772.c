void get_node_info(struct f2fs_sb_info *sbi, nid_t nid, struct node_info *ni)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct curseg_info *curseg = CURSEG_I(sbi, CURSEG_HOT_DATA);
	struct f2fs_journal *journal = curseg->journal;
	nid_t start_nid = START_NID(nid);
	struct f2fs_nat_block *nat_blk;
	struct page *page = NULL;
	struct f2fs_nat_entry ne;
	struct nat_entry *e;
	int i;

	ni->nid = nid;

	/* Check nat cache */
	down_read(&nm_i->nat_tree_lock);
	e = __lookup_nat_cache(nm_i, nid);
	if (e) {
		ni->ino = nat_get_ino(e);
		ni->blk_addr = nat_get_blkaddr(e);
		ni->version = nat_get_version(e);
		up_read(&nm_i->nat_tree_lock);
		return;
	}

	memset(&ne, 0, sizeof(struct f2fs_nat_entry));

	/* Check current segment summary */
	down_read(&curseg->journal_rwsem);
	i = lookup_journal_in_cursum(journal, NAT_JOURNAL, nid, 0);
	if (i >= 0) {
		ne = nat_in_journal(journal, i);
		node_info_from_raw_nat(ni, &ne);
	}
	up_read(&curseg->journal_rwsem);
	if (i >= 0)
		goto cache;

	/* Fill node_info from nat page */
	page = get_current_nat_page(sbi, start_nid);
	nat_blk = (struct f2fs_nat_block *)page_address(page);
	ne = nat_blk->entries[nid - start_nid];
	node_info_from_raw_nat(ni, &ne);
	f2fs_put_page(page, 1);
cache:
	up_read(&nm_i->nat_tree_lock);
	/* cache nat entry */
	down_write(&nm_i->nat_tree_lock);
	cache_nat_entry(sbi, nid, &ne);
	up_write(&nm_i->nat_tree_lock);
}
