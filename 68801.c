int try_to_free_nids(struct f2fs_sb_info *sbi, int nr_shrink)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct free_nid *i, *next;
	int nr = nr_shrink;

	if (nm_i->nid_cnt[FREE_NID_LIST] <= MAX_FREE_NIDS)
		return 0;

	if (!mutex_trylock(&nm_i->build_lock))
		return 0;

	spin_lock(&nm_i->nid_list_lock);
	list_for_each_entry_safe(i, next, &nm_i->nid_list[FREE_NID_LIST],
									list) {
		if (nr_shrink <= 0 ||
				nm_i->nid_cnt[FREE_NID_LIST] <= MAX_FREE_NIDS)
			break;

		__remove_nid_from_list(sbi, i, FREE_NID_LIST, false);
		kmem_cache_free(free_nid_slab, i);
		nr_shrink--;
	}
	spin_unlock(&nm_i->nid_list_lock);
	mutex_unlock(&nm_i->build_lock);

	return nr - nr_shrink;
}
