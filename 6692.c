void gf_fs_post_task_ex(GF_FilterSession *fsess, gf_fs_task_callback task_fun, GF_Filter *filter, GF_FilterPid *pid, const char *log_name, void *udta, Bool is_configure, Bool force_direct_call)
{
	GF_FSTask *task;
	Bool force_main_thread = GF_FALSE;
	Bool notified = GF_FALSE;

	assert(fsess);
	assert(task_fun);

	//only flatten calls if in main thread (we still have some broken filters using threading that could trigger tasks)
	if ((force_direct_call || fsess->direct_mode)
		&& (!filter || !filter->in_process)
		&& fsess->tasks_in_process
		&& (gf_th_id()==fsess->main_th.th_id)
	) {
		GF_FSTask atask;
		u64 task_time = gf_sys_clock_high_res();
		memset(&atask, 0, sizeof(GF_FSTask));
		atask.filter = filter;
		atask.pid = pid;
		atask.run_task = task_fun;
		atask.log_name = log_name;
		atask.udta = udta;
		GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread 0 task#%d %p executing Filter %s::%s (%d tasks pending)\n", fsess->main_th.nb_tasks, &atask, filter ? filter->name : "none", log_name, fsess->tasks_pending));
		if (filter)
			filter->scheduled_for_next_task = GF_TRUE;
		task_fun(&atask);
		filter = atask.filter;
		if (filter) {
			filter->time_process += gf_sys_clock_high_res() - task_time;
			filter->scheduled_for_next_task = GF_FALSE;
			filter->nb_tasks_done++;
		}
		if (!atask.requeue_request)
			return;
		//asked to requeue the task, post it
	}

	/*this was a gf_filter_process_task request but direct call could not be done or requeue is requested.
	process_task_queued was incremented by caller without checking for existing process task
		- If the task was not treated, dec / inc will give the same state, undo process_task_queued increment
		- If the task was requeued, dec will undo the increment done when requeing the task in gf_filter_check_pending_tasks

	In both cases, inc will redo the same logic as in gf_filter_post_process_task_internal, not creating task if gf_filter_process_task is
	already scheduled for the filter

	We must use safe_int_dec/safe_int_inc here for multi thread cases - cf issue #1778
	*/
	if (force_direct_call) {
		assert(filter);
		safe_int_dec(&filter->process_task_queued);
		if (safe_int_inc(&filter->process_task_queued) > 1) {
			return;
		}
	}
	task = gf_fq_pop(fsess->tasks_reservoir);

	if (!task) {
		GF_SAFEALLOC(task, GF_FSTask);
		if (!task) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_SCHEDULER, ("No more memory to post new task\n"));
			return;
		}
	}
	task->filter = filter;
	task->pid = pid;
	task->run_task = task_fun;
	task->log_name = log_name;
	task->udta = udta;

	if (filter && is_configure) {
		if (filter->freg->flags & GF_FS_REG_CONFIGURE_MAIN_THREAD)
			force_main_thread = GF_TRUE;
	}

	if (filter) {
		gf_mx_p(filter->tasks_mx);

		//no tasks and not scheduled
		if (! filter->scheduled_for_next_task && !gf_fq_count(filter->tasks)) {
			notified = task->notified = GF_TRUE;

			if (!force_main_thread)
				force_main_thread = (filter->main_thread_forced || (filter->freg->flags & GF_FS_REG_MAIN_THREAD)) ? GF_TRUE : GF_FALSE;
		} else if (force_main_thread) {
			force_main_thread = GF_FALSE;
			if (filter->process_th_id && (fsess->main_th.th_id != filter->process_th_id)) {
				GF_LOG(GF_LOG_ERROR, GF_LOG_SCHEDULER, ("Cannot post task to main thread, filter is already scheduled\n"));
			}
		}
		if (!force_main_thread)
			task->blocking = (filter->freg->flags & GF_FS_REG_BLOCKING) ? GF_TRUE : GF_FALSE;

		gf_fq_add(filter->tasks, task);
		gf_mx_v(filter->tasks_mx);

		GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u Posted task %p Filter %s::%s (%d (%d) pending, %d process tasks) on %s task list\n", gf_th_id(), task, filter->name, task->log_name, fsess->tasks_pending, gf_fq_count(filter->tasks), filter->process_task_queued, task->notified ? (force_main_thread ? "main" : "secondary") : "filter"));
	} else {
		task->notified = notified = GF_TRUE;
		GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u Posted filter-less task %s (%d pending) on secondary task list\n", gf_th_id(), task->log_name, fsess->tasks_pending));
	}

	//WARNING, do not use task->notified since the task may have been posted to the filter task list and may already have been swapped
	//with a different value !
	if (notified) {
#ifdef CHECK_TASK_LIST_INTEGRITY
		check_task_list(fsess->main_thread_tasks, task);
		check_task_list(fsess->tasks, task);
		check_task_list(fsess->tasks_reservoir, task);
#endif
		assert(task->run_task);
		if (filter) {
			GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u posting filter task, scheduled_for_next_task %d\n", gf_th_id(), filter->scheduled_for_next_task));
			assert(!filter->scheduled_for_next_task);
		}

		//notify/count tasks posted on the main task or regular task lists
		safe_int_inc(&fsess->tasks_pending);
		if (filter && force_main_thread) {
			gf_fq_add(fsess->main_thread_tasks, task);
			gf_fs_sema_io(fsess, GF_TRUE, GF_TRUE);
		} else {
			assert(task->run_task);
			gf_fq_add(fsess->tasks, task);
			gf_fs_sema_io(fsess, GF_TRUE, GF_FALSE);
		}
	}
}