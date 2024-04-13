static int f2fs_quota_on_mount(struct f2fs_sb_info *sbi, int type)
{
	return dquot_quota_on_mount(sbi->sb, sbi->s_qf_names[type],
						sbi->s_jquota_fmt, type);
}
