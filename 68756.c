void alloc_nid_done(struct f2fs_sb_info *sbi, nid_t nid)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct free_nid *i;

	spin_lock(&nm_i->nid_list_lock);
	i = __lookup_free_nid_list(nm_i, nid);
	f2fs_bug_on(sbi, !i);
	__remove_nid_from_list(sbi, i, ALLOC_NID_LIST, false);
	spin_unlock(&nm_i->nid_list_lock);

	kmem_cache_free(free_nid_slab, i);
}
