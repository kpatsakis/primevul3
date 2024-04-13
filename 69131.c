void f2fs_enable_quota_files(struct f2fs_sb_info *sbi)
{
	int i, ret;

	for (i = 0; i < MAXQUOTAS; i++) {
		if (sbi->s_qf_names[i]) {
			ret = f2fs_quota_on_mount(sbi, i);
			if (ret < 0)
				f2fs_msg(sbi->sb, KERN_ERR,
					"Cannot turn on journaled "
					"quota: error %d", ret);
		}
	}
}
