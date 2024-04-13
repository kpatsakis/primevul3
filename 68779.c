bool is_checkpointed_node(struct f2fs_sb_info *sbi, nid_t nid)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct nat_entry *e;
	bool is_cp = true;

	down_read(&nm_i->nat_tree_lock);
	e = __lookup_nat_cache(nm_i, nid);
	if (e && !get_nat_flag(e, IS_CHECKPOINTED))
		is_cp = false;
	up_read(&nm_i->nat_tree_lock);
	return is_cp;
}
