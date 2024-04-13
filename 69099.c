static void __wait_discard_cmd(struct f2fs_sb_info *sbi, bool wait_cond)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	struct list_head *wait_list = &(dcc->wait_list);
	struct discard_cmd *dc, *tmp;
	bool need_wait;

next:
	need_wait = false;

	mutex_lock(&dcc->cmd_lock);
	list_for_each_entry_safe(dc, tmp, wait_list, list) {
		if (!wait_cond || (dc->state == D_DONE && !dc->ref)) {
			wait_for_completion_io(&dc->wait);
			__remove_discard_cmd(sbi, dc);
		} else {
			dc->ref++;
			need_wait = true;
			break;
		}
	}
	mutex_unlock(&dcc->cmd_lock);

	if (need_wait) {
		__wait_one_discard_bio(sbi, dc);
		goto next;
	}
}
