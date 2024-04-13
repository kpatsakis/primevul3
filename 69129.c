static int f2fs_check_quota_options(struct f2fs_sb_info *sbi)
{
	/*
	 * We do the test below only for project quotas. 'usrquota' and
	 * 'grpquota' mount options are allowed even without quota feature
	 * to support legacy quotas in quota files.
	 */
	if (test_opt(sbi, PRJQUOTA) && !f2fs_sb_has_project_quota(sbi->sb)) {
		f2fs_msg(sbi->sb, KERN_ERR, "Project quota feature not enabled. "
			 "Cannot enable project quota enforcement.");
		return -1;
	}
	if (sbi->s_qf_names[USRQUOTA] || sbi->s_qf_names[GRPQUOTA] ||
			sbi->s_qf_names[PRJQUOTA]) {
		if (test_opt(sbi, USRQUOTA) && sbi->s_qf_names[USRQUOTA])
			clear_opt(sbi, USRQUOTA);

		if (test_opt(sbi, GRPQUOTA) && sbi->s_qf_names[GRPQUOTA])
			clear_opt(sbi, GRPQUOTA);

		if (test_opt(sbi, PRJQUOTA) && sbi->s_qf_names[PRJQUOTA])
			clear_opt(sbi, PRJQUOTA);

		if (test_opt(sbi, GRPQUOTA) || test_opt(sbi, USRQUOTA) ||
				test_opt(sbi, PRJQUOTA)) {
			f2fs_msg(sbi->sb, KERN_ERR, "old and new quota "
					"format mixing");
			return -1;
		}

		if (!sbi->s_jquota_fmt) {
			f2fs_msg(sbi->sb, KERN_ERR, "journaled quota format "
					"not specified");
			return -1;
		}
	}
	return 0;
}
