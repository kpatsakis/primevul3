static void cache_nat_entry(struct f2fs_sb_info *sbi, nid_t nid,
						struct f2fs_nat_entry *ne)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct nat_entry *e;

	e = __lookup_nat_cache(nm_i, nid);
	if (!e) {
		e = grab_nat_entry(nm_i, nid, false);
		if (e)
			node_info_from_raw_nat(&e->ni, ne);
	} else {
		f2fs_bug_on(sbi, nat_get_ino(e) != le32_to_cpu(ne->ino) ||
				nat_get_blkaddr(e) !=
					le32_to_cpu(ne->block_addr) ||
				nat_get_version(e) != ne->version);
	}
}
