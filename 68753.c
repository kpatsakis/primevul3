static void __set_nat_cache_dirty(struct f2fs_nm_info *nm_i,
						struct nat_entry *ne)
{
	nid_t set = NAT_BLOCK_OFFSET(ne->ni.nid);
	struct nat_entry_set *head;

	if (get_nat_flag(ne, IS_DIRTY))
		return;

	head = radix_tree_lookup(&nm_i->nat_set_root, set);
	if (!head) {
		head = f2fs_kmem_cache_alloc(nat_entry_set_slab, GFP_NOFS);

		INIT_LIST_HEAD(&head->entry_list);
		INIT_LIST_HEAD(&head->set_list);
		head->set = set;
		head->entry_cnt = 0;
		f2fs_radix_tree_insert(&nm_i->nat_set_root, set, head);
	}
	list_move_tail(&ne->list, &head->entry_list);
	nm_i->dirty_nat_cnt++;
	head->entry_cnt++;
	set_nat_flag(ne, IS_DIRTY, true);
}
