static void kill_f2fs_super(struct super_block *sb)
{
	if (sb->s_root) {
		set_sbi_flag(F2FS_SB(sb), SBI_IS_CLOSE);
		stop_gc_thread(F2FS_SB(sb));
		stop_discard_thread(F2FS_SB(sb));
	}
	kill_block_super(sb);
}
