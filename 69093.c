static void __drop_discard_cmd(struct f2fs_sb_info *sbi)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	struct list_head *pend_list;
	struct discard_cmd *dc, *tmp;
	int i;

	mutex_lock(&dcc->cmd_lock);
	for (i = MAX_PLIST_NUM - 1; i >= 0; i--) {
		pend_list = &dcc->pend_list[i];
		list_for_each_entry_safe(dc, tmp, pend_list, list) {
			f2fs_bug_on(sbi, dc->state != D_PREP);
			__remove_discard_cmd(sbi, dc);
		}
	}
	mutex_unlock(&dcc->cmd_lock);
}
