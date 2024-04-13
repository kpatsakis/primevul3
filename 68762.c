void destroy_node_manager(struct f2fs_sb_info *sbi)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct free_nid *i, *next_i;
	struct nat_entry *natvec[NATVEC_SIZE];
	struct nat_entry_set *setvec[SETVEC_SIZE];
	nid_t nid = 0;
	unsigned int found;

	if (!nm_i)
		return;

	/* destroy free nid list */
	spin_lock(&nm_i->nid_list_lock);
	list_for_each_entry_safe(i, next_i, &nm_i->nid_list[FREE_NID_LIST],
									list) {
		__remove_nid_from_list(sbi, i, FREE_NID_LIST, false);
		spin_unlock(&nm_i->nid_list_lock);
		kmem_cache_free(free_nid_slab, i);
		spin_lock(&nm_i->nid_list_lock);
	}
	f2fs_bug_on(sbi, nm_i->nid_cnt[FREE_NID_LIST]);
	f2fs_bug_on(sbi, nm_i->nid_cnt[ALLOC_NID_LIST]);
	f2fs_bug_on(sbi, !list_empty(&nm_i->nid_list[ALLOC_NID_LIST]));
	spin_unlock(&nm_i->nid_list_lock);

	/* destroy nat cache */
	down_write(&nm_i->nat_tree_lock);
	while ((found = __gang_lookup_nat_cache(nm_i,
					nid, NATVEC_SIZE, natvec))) {
		unsigned idx;

		nid = nat_get_nid(natvec[found - 1]) + 1;
		for (idx = 0; idx < found; idx++)
			__del_from_nat_cache(nm_i, natvec[idx]);
	}
	f2fs_bug_on(sbi, nm_i->nat_cnt);

	/* destroy nat set cache */
	nid = 0;
	while ((found = __gang_lookup_nat_set(nm_i,
					nid, SETVEC_SIZE, setvec))) {
		unsigned idx;

		nid = setvec[found - 1]->set + 1;
		for (idx = 0; idx < found; idx++) {
			/* entry_cnt is not zero, when cp_error was occurred */
			f2fs_bug_on(sbi, !list_empty(&setvec[idx]->entry_list));
			radix_tree_delete(&nm_i->nat_set_root, setvec[idx]->set);
			kmem_cache_free(nat_entry_set_slab, setvec[idx]);
		}
	}
	up_write(&nm_i->nat_tree_lock);

	kvfree(nm_i->nat_block_bitmap);
	kvfree(nm_i->free_nid_bitmap);
	kvfree(nm_i->free_nid_count);

	kfree(nm_i->nat_bitmap);
	kfree(nm_i->nat_bits);
#ifdef CONFIG_F2FS_CHECK_FS
	kfree(nm_i->nat_bitmap_mir);
#endif
	sbi->nm_info = NULL;
	kfree(nm_i);
}
