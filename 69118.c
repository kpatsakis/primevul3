 static void mark_discard_range_all(struct f2fs_sb_info *sbi)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	int i;

	mutex_lock(&dcc->cmd_lock);
	for (i = 0; i < MAX_PLIST_NUM; i++)
		dcc->pend_list_tag[i] |= P_TRIM;
	mutex_unlock(&dcc->cmd_lock);
}
