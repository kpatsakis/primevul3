static struct free_nid *__lookup_free_nid_list(struct f2fs_nm_info *nm_i,
						nid_t n)
{
	return radix_tree_lookup(&nm_i->free_nid_root, n);
}
