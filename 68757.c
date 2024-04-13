void alloc_nid_failed(struct f2fs_sb_info *sbi, nid_t nid)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct free_nid *i;
	bool need_free = false;

	if (!nid)
		return;

	spin_lock(&nm_i->nid_list_lock);
	i = __lookup_free_nid_list(nm_i, nid);
	f2fs_bug_on(sbi, !i);

	if (!available_free_memory(sbi, FREE_NIDS)) {
		__remove_nid_from_list(sbi, i, ALLOC_NID_LIST, false);
		need_free = true;
	} else {
		__remove_nid_from_list(sbi, i, ALLOC_NID_LIST, true);
		i->state = NID_NEW;
		__insert_nid_to_list(sbi, i, FREE_NID_LIST, false);
	}

	nm_i->available_nids++;

	update_free_nid_bitmap(sbi, nid, true, false);

	spin_unlock(&nm_i->nid_list_lock);

	if (need_free)
		kmem_cache_free(free_nid_slab, i);
}
