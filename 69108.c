static int create_discard_cmd_control(struct f2fs_sb_info *sbi)
{
	dev_t dev = sbi->sb->s_bdev->bd_dev;
	struct discard_cmd_control *dcc;
	int err = 0, i;

	if (SM_I(sbi)->dcc_info) {
		dcc = SM_I(sbi)->dcc_info;
		goto init_thread;
	}

	dcc = kzalloc(sizeof(struct discard_cmd_control), GFP_KERNEL);
	if (!dcc)
		return -ENOMEM;

	dcc->discard_granularity = DEFAULT_DISCARD_GRANULARITY;
	INIT_LIST_HEAD(&dcc->entry_list);
	for (i = 0; i < MAX_PLIST_NUM; i++) {
		INIT_LIST_HEAD(&dcc->pend_list[i]);
		if (i >= dcc->discard_granularity - 1)
			dcc->pend_list_tag[i] |= P_ACTIVE;
	}
	INIT_LIST_HEAD(&dcc->wait_list);
	mutex_init(&dcc->cmd_lock);
	atomic_set(&dcc->issued_discard, 0);
	atomic_set(&dcc->issing_discard, 0);
	atomic_set(&dcc->discard_cmd_cnt, 0);
	dcc->nr_discards = 0;
	dcc->max_discards = MAIN_SEGS(sbi) << sbi->log_blocks_per_seg;
	dcc->undiscard_blks = 0;
	dcc->root = RB_ROOT;

	init_waitqueue_head(&dcc->discard_wait_queue);
	SM_I(sbi)->dcc_info = dcc;
init_thread:
	dcc->f2fs_issue_discard = kthread_run(issue_discard_thread, sbi,
				"f2fs_discard-%u:%u", MAJOR(dev), MINOR(dev));
	if (IS_ERR(dcc->f2fs_issue_discard)) {
		err = PTR_ERR(dcc->f2fs_issue_discard);
		kfree(dcc);
		SM_I(sbi)->dcc_info = NULL;
		return err;
	}

	return err;
}
