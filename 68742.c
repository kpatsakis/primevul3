static void __build_free_nids(struct f2fs_sb_info *sbi, bool sync, bool mount)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct curseg_info *curseg = CURSEG_I(sbi, CURSEG_HOT_DATA);
	struct f2fs_journal *journal = curseg->journal;
	int i = 0;
	nid_t nid = nm_i->next_scan_nid;

	/* Enough entries */
	if (nm_i->nid_cnt[FREE_NID_LIST] >= NAT_ENTRY_PER_BLOCK)
		return;

	if (!sync && !available_free_memory(sbi, FREE_NIDS))
		return;

	if (!mount) {
		/* try to find free nids in free_nid_bitmap */
		scan_free_nid_bits(sbi);

		if (nm_i->nid_cnt[FREE_NID_LIST])
			return;
	}

	/* readahead nat pages to be scanned */
	ra_meta_pages(sbi, NAT_BLOCK_OFFSET(nid), FREE_NID_PAGES,
							META_NAT, true);

	down_read(&nm_i->nat_tree_lock);

	while (1) {
		struct page *page = get_current_nat_page(sbi, nid);

		scan_nat_page(sbi, page, nid);
		f2fs_put_page(page, 1);

		nid += (NAT_ENTRY_PER_BLOCK - (nid % NAT_ENTRY_PER_BLOCK));
		if (unlikely(nid >= nm_i->max_nid))
			nid = 0;

		if (++i >= FREE_NID_PAGES)
			break;
	}

	/* go to the next free nat pages to find free nids abundantly */
	nm_i->next_scan_nid = nid;

	/* find free nids from current sum_pages */
	down_read(&curseg->journal_rwsem);
	for (i = 0; i < nats_in_cursum(journal); i++) {
		block_t addr;

		addr = le32_to_cpu(nat_in_journal(journal, i).block_addr);
		nid = le32_to_cpu(nid_in_journal(journal, i));
		if (addr == NULL_ADDR)
			add_free_nid(sbi, nid, true);
		else
			remove_free_nid(sbi, nid);
	}
	up_read(&curseg->journal_rwsem);
	up_read(&nm_i->nat_tree_lock);

	ra_meta_pages(sbi, NAT_BLOCK_OFFSET(nm_i->next_scan_nid),
					nm_i->ra_nid_pages, META_NAT, false);
}
