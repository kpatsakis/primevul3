static int __insert_nid_to_list(struct f2fs_sb_info *sbi,
			struct free_nid *i, enum nid_list list, bool new)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);

	if (new) {
		int err = radix_tree_insert(&nm_i->free_nid_root, i->nid, i);
		if (err)
			return err;
	}

	f2fs_bug_on(sbi, list == FREE_NID_LIST ? i->state != NID_NEW :
						i->state != NID_ALLOC);
	nm_i->nid_cnt[list]++;
	list_add_tail(&i->list, &nm_i->nid_list[list]);
	return 0;
}
