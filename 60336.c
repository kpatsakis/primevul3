static int ext4_lazyinit_thread(void *arg)
{
	struct ext4_lazy_init *eli = (struct ext4_lazy_init *)arg;
	struct list_head *pos, *n;
	struct ext4_li_request *elr;
	unsigned long next_wakeup, cur;

	BUG_ON(NULL == eli);

cont_thread:
	while (true) {
		next_wakeup = MAX_JIFFY_OFFSET;

		mutex_lock(&eli->li_list_mtx);
		if (list_empty(&eli->li_request_list)) {
			mutex_unlock(&eli->li_list_mtx);
			goto exit_thread;
		}
		list_for_each_safe(pos, n, &eli->li_request_list) {
			int err = 0;
			int progress = 0;
			elr = list_entry(pos, struct ext4_li_request,
					 lr_request);

			if (time_before(jiffies, elr->lr_next_sched)) {
				if (time_before(elr->lr_next_sched, next_wakeup))
					next_wakeup = elr->lr_next_sched;
				continue;
			}
			if (down_read_trylock(&elr->lr_super->s_umount)) {
				if (sb_start_write_trylock(elr->lr_super)) {
					progress = 1;
					/*
					 * We hold sb->s_umount, sb can not
					 * be removed from the list, it is
					 * now safe to drop li_list_mtx
					 */
					mutex_unlock(&eli->li_list_mtx);
					err = ext4_run_li_request(elr);
					sb_end_write(elr->lr_super);
					mutex_lock(&eli->li_list_mtx);
					n = pos->next;
				}
				up_read((&elr->lr_super->s_umount));
			}
			/* error, remove the lazy_init job */
			if (err) {
				ext4_remove_li_request(elr);
				continue;
			}
			if (!progress) {
				elr->lr_next_sched = jiffies +
					(prandom_u32()
					 % (EXT4_DEF_LI_MAX_START_DELAY * HZ));
			}
			if (time_before(elr->lr_next_sched, next_wakeup))
				next_wakeup = elr->lr_next_sched;
		}
		mutex_unlock(&eli->li_list_mtx);

		try_to_freeze();

		cur = jiffies;
		if ((time_after_eq(cur, next_wakeup)) ||
		    (MAX_JIFFY_OFFSET == next_wakeup)) {
			cond_resched();
			continue;
		}

		schedule_timeout_interruptible(next_wakeup - cur);

		if (kthread_should_stop()) {
			ext4_clear_request_list();
			goto exit_thread;
		}
	}

exit_thread:
	/*
	 * It looks like the request list is empty, but we need
	 * to check it under the li_list_mtx lock, to prevent any
	 * additions into it, and of course we should lock ext4_li_mtx
	 * to atomically free the list and ext4_li_info, because at
	 * this point another ext4 filesystem could be registering
	 * new one.
	 */
	mutex_lock(&ext4_li_mtx);
	mutex_lock(&eli->li_list_mtx);
	if (!list_empty(&eli->li_request_list)) {
		mutex_unlock(&eli->li_list_mtx);
		mutex_unlock(&ext4_li_mtx);
		goto cont_thread;
	}
	mutex_unlock(&eli->li_list_mtx);
	kfree(ext4_li_info);
	ext4_li_info = NULL;
	mutex_unlock(&ext4_li_mtx);

	return 0;
}
