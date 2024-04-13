bool alloc_nid(struct f2fs_sb_info *sbi, nid_t *nid)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct free_nid *i = NULL;
retry:
#ifdef CONFIG_F2FS_FAULT_INJECTION
	if (time_to_inject(sbi, FAULT_ALLOC_NID)) {
		f2fs_show_injection_info(FAULT_ALLOC_NID);
		return false;
	}
#endif
	spin_lock(&nm_i->nid_list_lock);

	if (unlikely(nm_i->available_nids == 0)) {
		spin_unlock(&nm_i->nid_list_lock);
		return false;
	}

	/* We should not use stale free nids created by build_free_nids */
	if (nm_i->nid_cnt[FREE_NID_LIST] && !on_build_free_nids(nm_i)) {
		f2fs_bug_on(sbi, list_empty(&nm_i->nid_list[FREE_NID_LIST]));
		i = list_first_entry(&nm_i->nid_list[FREE_NID_LIST],
					struct free_nid, list);
		*nid = i->nid;

		__remove_nid_from_list(sbi, i, FREE_NID_LIST, true);
		i->state = NID_ALLOC;
		__insert_nid_to_list(sbi, i, ALLOC_NID_LIST, false);
		nm_i->available_nids--;

		update_free_nid_bitmap(sbi, *nid, false, false);

		spin_unlock(&nm_i->nid_list_lock);
		return true;
	}
	spin_unlock(&nm_i->nid_list_lock);

	/* Let's scan nat pages and its caches to get free nids */
	build_free_nids(sbi, true, false);
	goto retry;
}
