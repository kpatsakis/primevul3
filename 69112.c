int f2fs_issue_flush(struct f2fs_sb_info *sbi)
{
	struct flush_cmd_control *fcc = SM_I(sbi)->fcc_info;
	struct flush_cmd cmd;
	int ret;

	if (test_opt(sbi, NOBARRIER))
		return 0;

	if (!test_opt(sbi, FLUSH_MERGE)) {
		ret = submit_flush_wait(sbi);
		atomic_inc(&fcc->issued_flush);
		return ret;
	}

	if (atomic_inc_return(&fcc->issing_flush) == 1) {
		ret = submit_flush_wait(sbi);
		atomic_dec(&fcc->issing_flush);

		atomic_inc(&fcc->issued_flush);
		return ret;
	}

	init_completion(&cmd.wait);

	llist_add(&cmd.llnode, &fcc->issue_list);

	/* update issue_list before we wake up issue_flush thread */
	smp_mb();

	if (waitqueue_active(&fcc->flush_wait_queue))
		wake_up(&fcc->flush_wait_queue);

	if (fcc->f2fs_issue_flush) {
		wait_for_completion(&cmd.wait);
		atomic_dec(&fcc->issing_flush);
	} else {
		struct llist_node *list;

		list = llist_del_all(&fcc->issue_list);
		if (!list) {
			wait_for_completion(&cmd.wait);
			atomic_dec(&fcc->issing_flush);
		} else {
			struct flush_cmd *tmp, *next;

			ret = submit_flush_wait(sbi);

			llist_for_each_entry_safe(tmp, next, list, llnode) {
				if (tmp == &cmd) {
					cmd.ret = ret;
					atomic_dec(&fcc->issing_flush);
					continue;
				}
				tmp->ret = ret;
				complete(&tmp->wait);
			}
		}
	}

	return cmd.ret;
}
