static int f2fs_clear_qf_name(struct super_block *sb, int qtype)
{
	struct f2fs_sb_info *sbi = F2FS_SB(sb);

	if (sb_any_quota_loaded(sb) && sbi->s_qf_names[qtype]) {
		f2fs_msg(sb, KERN_ERR, "Cannot change journaled quota options"
			" when quota turned on");
		return -EINVAL;
	}
	kfree(sbi->s_qf_names[qtype]);
	sbi->s_qf_names[qtype] = NULL;
	return 0;
}
