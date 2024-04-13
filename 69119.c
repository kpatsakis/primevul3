bool need_SSR(struct f2fs_sb_info *sbi)
{
	int node_secs = get_blocktype_secs(sbi, F2FS_DIRTY_NODES);
	int dent_secs = get_blocktype_secs(sbi, F2FS_DIRTY_DENTS);
	int imeta_secs = get_blocktype_secs(sbi, F2FS_DIRTY_IMETA);

	if (test_opt(sbi, LFS))
		return false;
	if (sbi->gc_thread && sbi->gc_thread->gc_urgent)
		return true;

	return free_sections(sbi) <= (node_secs + 2 * dent_secs + imeta_secs +
						2 * reserved_sections(sbi));
}
