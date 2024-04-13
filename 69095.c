static int __issue_discard_cmd(struct f2fs_sb_info *sbi, bool issue_cond)
{
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	struct list_head *pend_list;
	struct discard_cmd *dc, *tmp;
	struct blk_plug plug;
	int iter = 0, issued = 0;
	int i;
	bool io_interrupted = false;

	mutex_lock(&dcc->cmd_lock);
	f2fs_bug_on(sbi,
		!__check_rb_tree_consistence(sbi, &dcc->root));
	blk_start_plug(&plug);
	for (i = MAX_PLIST_NUM - 1;
			i >= 0 && plist_issue(dcc->pend_list_tag[i]); i--) {
		pend_list = &dcc->pend_list[i];
		list_for_each_entry_safe(dc, tmp, pend_list, list) {
			f2fs_bug_on(sbi, dc->state != D_PREP);

			/* Hurry up to finish fstrim */
			if (dcc->pend_list_tag[i] & P_TRIM) {
				__submit_discard_cmd(sbi, dc);
				issued++;

				if (fatal_signal_pending(current))
					break;
				continue;
			}

			if (!issue_cond) {
				__submit_discard_cmd(sbi, dc);
				issued++;
				continue;
			}

			if (is_idle(sbi)) {
				__submit_discard_cmd(sbi, dc);
				issued++;
			} else {
				io_interrupted = true;
			}

			if (++iter >= DISCARD_ISSUE_RATE)
				goto out;
		}
		if (list_empty(pend_list) && dcc->pend_list_tag[i] & P_TRIM)
			dcc->pend_list_tag[i] &= (~P_TRIM);
	}
out:
	blk_finish_plug(&plug);
	mutex_unlock(&dcc->cmd_lock);

	if (!issued && io_interrupted)
		issued = -1;

	return issued;
}
