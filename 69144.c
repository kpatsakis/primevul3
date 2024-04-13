static int f2fs_set_qf_name(struct super_block *sb, int qtype,
							substring_t *args)
{
	struct f2fs_sb_info *sbi = F2FS_SB(sb);
	char *qname;
	int ret = -EINVAL;

	if (sb_any_quota_loaded(sb) && !sbi->s_qf_names[qtype]) {
		f2fs_msg(sb, KERN_ERR,
			"Cannot change journaled "
			"quota options when quota turned on");
		return -EINVAL;
	}
	qname = match_strdup(args);
	if (!qname) {
		f2fs_msg(sb, KERN_ERR,
			"Not enough memory for storing quotafile name");
		return -EINVAL;
	}
	if (sbi->s_qf_names[qtype]) {
		if (strcmp(sbi->s_qf_names[qtype], qname) == 0)
			ret = 0;
		else
			f2fs_msg(sb, KERN_ERR,
				 "%s quota file already specified",
				 QTYPE2NAME(qtype));
		goto errout;
	}
	if (strchr(qname, '/')) {
		f2fs_msg(sb, KERN_ERR,
			"quotafile must be on filesystem root");
		goto errout;
	}
	sbi->s_qf_names[qtype] = qname;
	set_opt(sbi, QUOTA);
	return 0;
errout:
	kfree(qname);
	return ret;
}
