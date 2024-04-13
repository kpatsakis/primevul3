bool available_free_memory(struct f2fs_sb_info *sbi, int type)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct sysinfo val;
	unsigned long avail_ram;
	unsigned long mem_size = 0;
	bool res = false;

	si_meminfo(&val);

	/* only uses low memory */
	avail_ram = val.totalram - val.totalhigh;

	/*
	 * give 25%, 25%, 50%, 50%, 50% memory for each components respectively
	 */
	if (type == FREE_NIDS) {
		mem_size = (nm_i->nid_cnt[FREE_NID_LIST] *
				sizeof(struct free_nid)) >> PAGE_SHIFT;
		res = mem_size < ((avail_ram * nm_i->ram_thresh / 100) >> 2);
	} else if (type == NAT_ENTRIES) {
		mem_size = (nm_i->nat_cnt * sizeof(struct nat_entry)) >>
							PAGE_SHIFT;
		res = mem_size < ((avail_ram * nm_i->ram_thresh / 100) >> 2);
		if (excess_cached_nats(sbi))
			res = false;
	} else if (type == DIRTY_DENTS) {
		if (sbi->sb->s_bdi->wb.dirty_exceeded)
			return false;
		mem_size = get_pages(sbi, F2FS_DIRTY_DENTS);
		res = mem_size < ((avail_ram * nm_i->ram_thresh / 100) >> 1);
	} else if (type == INO_ENTRIES) {
		int i;

		for (i = 0; i <= UPDATE_INO; i++)
			mem_size += sbi->im[i].ino_num *
						sizeof(struct ino_entry);
		mem_size >>= PAGE_SHIFT;
		res = mem_size < ((avail_ram * nm_i->ram_thresh / 100) >> 1);
	} else if (type == EXTENT_CACHE) {
		mem_size = (atomic_read(&sbi->total_ext_tree) *
				sizeof(struct extent_tree) +
				atomic_read(&sbi->total_ext_node) *
				sizeof(struct extent_node)) >> PAGE_SHIFT;
		res = mem_size < ((avail_ram * nm_i->ram_thresh / 100) >> 1);
	} else {
		if (!sbi->sb->s_bdi->wb.dirty_exceeded)
			return true;
	}
	return res;
}
