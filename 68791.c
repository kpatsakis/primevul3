 static void remove_free_nid(struct f2fs_sb_info *sbi, nid_t nid)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct free_nid *i;
	bool need_free = false;

	spin_lock(&nm_i->nid_list_lock);
	i = __lookup_free_nid_list(nm_i, nid);
	if (i && i->state == NID_NEW) {
		__remove_nid_from_list(sbi, i, FREE_NID_LIST, false);
		need_free = true;
	}
	spin_unlock(&nm_i->nid_list_lock);

	if (need_free)
		kmem_cache_free(free_nid_slab, i);
}
