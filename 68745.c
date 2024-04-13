static void __flush_nat_entry_set(struct f2fs_sb_info *sbi,
		struct nat_entry_set *set, struct cp_control *cpc)
{
	struct curseg_info *curseg = CURSEG_I(sbi, CURSEG_HOT_DATA);
	struct f2fs_journal *journal = curseg->journal;
	nid_t start_nid = set->set * NAT_ENTRY_PER_BLOCK;
	bool to_journal = true;
	struct f2fs_nat_block *nat_blk;
	struct nat_entry *ne, *cur;
	struct page *page = NULL;

	/*
	 * there are two steps to flush nat entries:
	 * #1, flush nat entries to journal in current hot data summary block.
	 * #2, flush nat entries to nat page.
	 */
	if (enabled_nat_bits(sbi, cpc) ||
		!__has_cursum_space(journal, set->entry_cnt, NAT_JOURNAL))
		to_journal = false;

	if (to_journal) {
		down_write(&curseg->journal_rwsem);
	} else {
		page = get_next_nat_page(sbi, start_nid);
		nat_blk = page_address(page);
		f2fs_bug_on(sbi, !nat_blk);
	}

	/* flush dirty nats in nat entry set */
	list_for_each_entry_safe(ne, cur, &set->entry_list, list) {
		struct f2fs_nat_entry *raw_ne;
		nid_t nid = nat_get_nid(ne);
		int offset;

		if (nat_get_blkaddr(ne) == NEW_ADDR)
			continue;

		if (to_journal) {
			offset = lookup_journal_in_cursum(journal,
							NAT_JOURNAL, nid, 1);
			f2fs_bug_on(sbi, offset < 0);
			raw_ne = &nat_in_journal(journal, offset);
			nid_in_journal(journal, offset) = cpu_to_le32(nid);
		} else {
			raw_ne = &nat_blk->entries[nid - start_nid];
		}
		raw_nat_from_node_info(raw_ne, &ne->ni);
		nat_reset_flag(ne);
		__clear_nat_cache_dirty(NM_I(sbi), set, ne);
		if (nat_get_blkaddr(ne) == NULL_ADDR) {
			add_free_nid(sbi, nid, false);
			spin_lock(&NM_I(sbi)->nid_list_lock);
			NM_I(sbi)->available_nids++;
			update_free_nid_bitmap(sbi, nid, true, false);
			spin_unlock(&NM_I(sbi)->nid_list_lock);
		} else {
			spin_lock(&NM_I(sbi)->nid_list_lock);
			update_free_nid_bitmap(sbi, nid, false, false);
			spin_unlock(&NM_I(sbi)->nid_list_lock);
		}
	}

	if (to_journal) {
		up_write(&curseg->journal_rwsem);
	} else {
		__update_nat_bits(sbi, start_nid, page);
		f2fs_put_page(page, 1);
	}

	f2fs_bug_on(sbi, set->entry_cnt);

	radix_tree_delete(&NM_I(sbi)->nat_set_root, set->set);
	kmem_cache_free(nat_entry_set_slab, set);
}
