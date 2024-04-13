static void __clear_nat_cache_dirty(struct f2fs_nm_info *nm_i,
		struct nat_entry_set *set, struct nat_entry *ne)
{
	list_move_tail(&ne->list, &nm_i->nat_entries);
	set_nat_flag(ne, IS_DIRTY, false);
	set->entry_cnt--;
	nm_i->dirty_nat_cnt--;
}
