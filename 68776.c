static struct nat_entry *grab_nat_entry(struct f2fs_nm_info *nm_i, nid_t nid,
								bool no_fail)
{
	struct nat_entry *new;

	if (no_fail) {
		new = f2fs_kmem_cache_alloc(nat_entry_slab, GFP_NOFS);
		f2fs_radix_tree_insert(&nm_i->nat_root, nid, new);
	} else {
		new = kmem_cache_alloc(nat_entry_slab, GFP_NOFS);
		if (!new)
			return NULL;
		if (radix_tree_insert(&nm_i->nat_root, nid, new)) {
			kmem_cache_free(nat_entry_slab, new);
			return NULL;
		}
	}

	memset(new, 0, sizeof(struct nat_entry));
	nat_set_nid(new, nid);
	nat_reset_flag(new);
	list_add_tail(&new->list, &nm_i->nat_entries);
	nm_i->nat_cnt++;
	return new;
}
