static void destroy_discard_cmd_control(struct f2fs_sb_info *sbi)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;

	if (!dcc)
		return;

	stop_discard_thread(sbi);

	kfree(dcc);
	SM_I(sbi)->dcc_info = NULL;
}
