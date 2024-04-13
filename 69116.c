static int issue_discard_thread(void *data)
{
	struct f2fs_sb_info *sbi = data;
	struct discard_cmd_control *dcc = SM_I(sbi)->dcc_info;
	wait_queue_head_t *q = &dcc->discard_wait_queue;
	unsigned int wait_ms = DEF_MIN_DISCARD_ISSUE_TIME;
	int issued;

	set_freezable();

	do {
		wait_event_interruptible_timeout(*q,
				kthread_should_stop() || freezing(current) ||
				dcc->discard_wake,
				msecs_to_jiffies(wait_ms));
		if (try_to_freeze())
			continue;
		if (kthread_should_stop())
			return 0;

		if (dcc->discard_wake) {
			dcc->discard_wake = 0;
			if (sbi->gc_thread && sbi->gc_thread->gc_urgent)
				mark_discard_range_all(sbi);
		}

		sb_start_intwrite(sbi->sb);

		issued = __issue_discard_cmd(sbi, true);
		if (issued) {
			__wait_discard_cmd(sbi, true);
			wait_ms = DEF_MIN_DISCARD_ISSUE_TIME;
		} else {
			wait_ms = DEF_MAX_DISCARD_ISSUE_TIME;
		}

		sb_end_intwrite(sbi->sb);

	} while (!kthread_should_stop());
	return 0;
}
