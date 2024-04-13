static void __remove_discard_cmd(struct f2fs_sb_info *sbi,
							struct discard_cmd *dc)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;

	f2fs_bug_on(sbi, dc->ref);

	if (dc->error == -EOPNOTSUPP)
		dc->error = 0;

	if (dc->error)
		f2fs_msg(sbi->sb, KERN_INFO,
			"Issue discard(%u, %u, %u) failed, ret: %d",
			dc->lstart, dc->start, dc->len, dc->error);
	__detach_discard_cmd(dcc, dc);
}
