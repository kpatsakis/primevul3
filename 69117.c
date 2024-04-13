static int issue_flush_thread(void *data)
{
	struct f2fs_sb_info *sbi = data;
	struct flush_cmd_control *fcc = SM_I(sbi)->fcc_info;
	wait_queue_head_t *q = &fcc->flush_wait_queue;
repeat:
	if (kthread_should_stop())
		return 0;

	sb_start_intwrite(sbi->sb);

	if (!llist_empty(&fcc->issue_list)) {
		struct flush_cmd *cmd, *next;
		int ret;

		fcc->dispatch_list = llist_del_all(&fcc->issue_list);
		fcc->dispatch_list = llist_reverse_order(fcc->dispatch_list);

		ret = submit_flush_wait(sbi);
		atomic_inc(&fcc->issued_flush);

		llist_for_each_entry_safe(cmd, next,
					  fcc->dispatch_list, llnode) {
			cmd->ret = ret;
			complete(&cmd->wait);
		}
		fcc->dispatch_list = NULL;
	}

	sb_end_intwrite(sbi->sb);

	wait_event_interruptible(*q,
		kthread_should_stop() || !llist_empty(&fcc->issue_list));
	goto repeat;
}
