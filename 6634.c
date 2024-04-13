static u32 gf_fs_thread_proc(GF_SessionThread *sess_thread)
{
	GF_FilterSession *fsess = sess_thread->fsess;
	u32 i, th_count = fsess->threads ? gf_list_count(fsess->threads) : 0;
	u32 thid =  1 + gf_list_find(fsess->threads, sess_thread);
	u64 enter_time = gf_sys_clock_high_res();
	Bool use_main_sema = thid ? GF_FALSE : GF_TRUE;
#ifndef GPAC_DISABLE_LOG
	u32 sys_thid = gf_th_id();
#endif
	u64 next_task_schedule_time = 0;
	Bool do_regulate = (fsess->flags & GF_FS_FLAG_NO_REGULATION) ? GF_FALSE : GF_TRUE;
	u32 consecutive_filter_tasks=0;
	Bool force_secondary_tasks = GF_FALSE;
	Bool skip_next_sema_wait = GF_FALSE;

	GF_Filter *current_filter = NULL;
	sess_thread->th_id = gf_th_id();

#ifndef GPAC_DISABLE_REMOTERY
	sess_thread->rmt_tasks=40;
	gf_rmt_set_thread_name(sess_thread->rmt_name);
#endif

	gf_rmt_begin(fs_thread, 0);

	safe_int_inc(&fsess->active_threads);

	if (!thid && fsess->no_main_thread) {
		GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Main thread proc enter\n"));
	}

	while (1) {
		Bool notified;
		Bool requeue = GF_FALSE;
		u64 active_start, task_time;
		GF_FSTask *task=NULL;
#ifdef CHECK_TASK_LIST_INTEGRITY
		GF_Filter *prev_current_filter = NULL;
		Bool skip_filter_task_check = GF_FALSE;
#endif

#ifndef GPAC_DISABLE_REMOTERY
		sess_thread->rmt_tasks--;
		if (!sess_thread->rmt_tasks) {
			gf_rmt_end();
			gf_rmt_begin(fs_thread, 0);
			sess_thread->rmt_tasks=40;
		}
#endif

		safe_int_dec(&fsess->active_threads);

		if (!skip_next_sema_wait && (current_filter==NULL)) {
			gf_rmt_begin(sema_wait, GF_RMT_AGGREGATE);
			GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u Waiting scheduler %s semaphore\n", sys_thid, use_main_sema ? "main" : "secondary"));
			//wait for something to be done
			gf_fs_sema_io(fsess, GF_FALSE, use_main_sema);
			consecutive_filter_tasks = 0;
			gf_rmt_end();
		}
		safe_int_inc(&fsess->active_threads);
		skip_next_sema_wait = GF_FALSE;

		active_start = gf_sys_clock_high_res();

		if (current_filter==NULL) {
			//main thread
			if (thid==0) {
				if (!force_secondary_tasks) {
					task = gf_fq_pop(fsess->main_thread_tasks);
				}
				if (!task) {
					task = gf_fq_pop(fsess->tasks);
					if (task && task->blocking) {
						gf_fq_add(fsess->tasks, task);
						task = NULL;
						gf_fs_sema_io(fsess, GF_TRUE, GF_FALSE);
					}
				}
				force_secondary_tasks = GF_FALSE;
			} else {
				task = gf_fq_pop(fsess->tasks);
			}
			if (task) {
				assert( task->run_task );
				assert( task->notified );
			}
		} else {
			//keep task in filter tasks list until done
			task = gf_fq_head(current_filter->tasks);
			if (task) {
				assert( task->run_task );
				assert( ! task->notified );
			}
		}

		if (!task) {
			u32 force_nb_notif = 0;
			next_task_schedule_time = 0;
			//no more task and EOS signal
			if (fsess->run_status != GF_OK)
				break;

			if (!fsess->tasks_pending && fsess->main_th.has_seen_eot) {
				//check all threads
				Bool all_done = GF_TRUE;

				for (i=0; i<th_count; i++) {
					GF_SessionThread *st = gf_list_get(fsess->threads, i);
					if (!st->has_seen_eot) {
						all_done = GF_FALSE;
						force_nb_notif++;
					}
				}
				if (all_done)
					break;
			}
			if (current_filter) {
				current_filter->scheduled_for_next_task = GF_FALSE;
				current_filter->process_th_id = 0;
				assert(current_filter->in_process);
				current_filter->in_process = GF_FALSE;
			}
			current_filter = NULL;
			sess_thread->active_time += gf_sys_clock_high_res() - active_start;


			//no pending tasks and first time main task queue is empty, flush to detect if we
			//are indeed done
			if (!fsess->tasks_pending && !fsess->tasks_in_process && !sess_thread->has_seen_eot && !gf_fq_count(fsess->tasks)) {
				//maybe last task, force a notify to check if we are truly done
				sess_thread->has_seen_eot = GF_TRUE;
				//not main thread and some tasks pending on main, notify only ourselves
				if (thid && gf_fq_count(fsess->main_thread_tasks)) {
					gf_fs_sema_io(fsess, GF_TRUE, use_main_sema);
				}
				//main thread exit probing, send a notify to main sema (for this thread), and N for the secondary one
				else {
					GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u notify scheduler main semaphore\n", gf_th_id()));
					gf_sema_notify(fsess->semaphore_main, 1);
					GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u notify scheduler secondary semaphore %d\n", gf_th_id(), th_count));
					gf_sema_notify(fsess->semaphore_other, th_count);
				}
			}
			//this thread and the main thread are done but we still have unfinished threads, re-notify everyone
			else if (!fsess->tasks_pending && fsess->main_th.has_seen_eot && force_nb_notif) {
				GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u notify scheduler main semaphore\n", gf_th_id()));
				gf_sema_notify(fsess->semaphore_main, 1);
				GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u notify scheduler secondary semaphore %d\n", gf_th_id(), th_count));
				gf_sema_notify(fsess->semaphore_other, th_count);
			}

			GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: no task available\n", sys_thid));

			//no main thread, return
			if (!thid && fsess->no_main_thread) {
				GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Main thread proc exit\n"));
				return 0;
			}

			if (do_regulate) {
				gf_sleep(0);
			}
			continue;
		}
#ifdef CHECK_TASK_LIST_INTEGRITY
		check_task_list(fsess->main_thread_tasks, task);
		check_task_list(fsess->tasks, task);
#endif
		if (current_filter) {
			assert(current_filter==task->filter);
		}
		current_filter = task->filter;

		//this is a crude way of scheduling the next task, we should
		//1- have a way to make sure we will not repost after a time-consuming task
		//2- have a way to wait for the given amount of time rather than just do a sema_wait/notify in loop
		if (task->schedule_next_time) {
			s64 now = gf_sys_clock_high_res();
			s64 diff = task->schedule_next_time;
			diff -= now;
			diff /= 1000;


			if (diff > 0) {
				GF_FSTask *next;
				s64 tdiff = diff;
				s64 ndiff = 0;

				//no filter, just reschedule the task
				if (!current_filter) {
#ifndef GPAC_DISABLE_LOG
					const char *task_log_name = task->log_name;
#endif
					next = gf_fq_head(fsess->tasks);
					next_task_schedule_time = task->schedule_next_time;
					assert(task->run_task);
#ifdef CHECK_TASK_LIST_INTEGRITY
					check_task_list(fsess->main_thread_tasks, task);
					check_task_list(fsess->tasks, task);
					check_task_list(fsess->tasks_reservoir, task);
#endif
					//tasks without filter are currently only posted to the secondary task list
					gf_fq_add(fsess->tasks, task);
					if (next) {
						if (next->schedule_next_time <= (u64) now) {
							GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s reposted, next task time ready for execution\n", sys_thid, task_log_name));

							skip_next_sema_wait = GF_TRUE;
							continue;
						}
						ndiff = next->schedule_next_time;
						ndiff -= now;
						ndiff /= 1000;
						if (ndiff<diff) {
							GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s scheduled after next task %s:%s (in %d ms vs %d ms)\n", sys_thid, task_log_name, next->log_name, next->filter ? next->filter->name : "", (s32) diff, (s32) ndiff));
							diff = ndiff;
						}
					}

					if (!do_regulate) {
						diff = 0;
					}

					if (diff && do_regulate) {
						if (diff > fsess->max_sleep)
							diff = fsess->max_sleep;
						if (th_count==0) {
							if ( gf_fq_count(fsess->tasks) > MONOTH_MIN_TASKS)
								diff = MONOTH_MIN_SLEEP;
						}
						GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s reposted, %s task scheduled after this task, sleeping for %d ms (task diff %d - next diff %d)\n", sys_thid, task_log_name, next ? "next" : "no", diff, tdiff, ndiff));
						gf_sleep((u32) diff);
					} else {
						GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s reposted, next task scheduled after this task, rerun\n", sys_thid, task_log_name));
					}
					skip_next_sema_wait = GF_TRUE;
					continue;
				}

				if (!task->filter->finalized) {
#ifdef CHECK_TASK_LIST_INTEGRITY
					next = gf_fq_head(current_filter->tasks);
					assert(next == task);
					check_task_list(fsess->main_thread_tasks, task);
					check_task_list(fsess->tasks_reservoir, task);
#endif

					//next in filter should be handled before this task, move task at the end of the filter task
					next = gf_fq_get(current_filter->tasks, 1);
					if (next && next->schedule_next_time < task->schedule_next_time) {
						if (task->notified) {
							assert(fsess->tasks_pending);
							safe_int_dec(&fsess->tasks_pending);
							task->notified = GF_FALSE;
						}
						GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s:%s reposted to filter task until task exec time is reached (%d us)\n", sys_thid, current_filter->name, task->log_name, (s32) (task->schedule_next_time - next->schedule_next_time) ));
						//remove task
						gf_fq_pop(current_filter->tasks);
						//and queue it after the next one
						gf_fq_add(current_filter->tasks, task);
						//and continue with the same filter
						continue;
					}
					//little optim here: if this is the main thread and we have other tasks pending
					//check the timing of tasks in the secondary list. If a task is present with smaller time than
					//the head of the main task, force a temporary swap to the secondary task list
					if (!thid && task->notified && diff>10) {
						next = gf_fq_head(fsess->tasks);
						if (next && !next->blocking) {
							u64 next_time_main = task->schedule_next_time;
							u64 next_time_secondary = next->schedule_next_time;
							//if we have several threads, also check the next task on the main task list
							// (different from secondary tasks in multithread case)
							if (th_count) {
								GF_FSTask *next_main = gf_fq_head(fsess->main_thread_tasks);
								if (next_main && (next_time_main > next_main->schedule_next_time))
									next_time_main = next_main->schedule_next_time;
							}
							
							if (next_time_secondary<next_time_main) {
								GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: forcing secondary task list on main - current task schedule time "LLU" (diff to now %d) vs next time secondary "LLU" (%s::%s)\n", sys_thid, task->schedule_next_time, (s32) diff, next_time_secondary, next->filter->freg->name, next->log_name));
								diff = 0;
								force_secondary_tasks = GF_TRUE;
							}
						}
					}

					//move task to main list
					if (!task->notified) {
						task->notified = GF_TRUE;
						safe_int_inc(&fsess->tasks_pending);
					}

					sess_thread->active_time += gf_sys_clock_high_res() - active_start;

					if (next_task_schedule_time && (next_task_schedule_time <= task->schedule_next_time)) {
						tdiff = next_task_schedule_time;
						tdiff -= now;
						if (tdiff < 0) tdiff=0;
						if (tdiff<diff) {
							GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: next task has earlier exec time than current task %s:%s, adjusting sleep (old %d - new %d)\n", sys_thid, current_filter->name, task->log_name, (s32) diff, (s32) tdiff));
							diff = tdiff;
						} else {
							GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: next task has earlier exec time#2 than current task %s:%s, adjusting sleep (old %d - new %d)\n", sys_thid, current_filter->name, task->log_name, (s32) diff, (s32) tdiff));

						}
					}

					if (do_regulate && diff) {
						if (diff > fsess->max_sleep)
							diff = fsess->max_sleep;
						if (th_count==0) {
							if ( gf_fq_count(fsess->tasks) > MONOTH_MIN_TASKS)
								diff = MONOTH_MIN_SLEEP;
						}
						GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s:%s postponed for %d ms (scheduled time "LLU" us, next task schedule "LLU" us)\n", sys_thid, current_filter->name, task->log_name, (s32) diff, task->schedule_next_time, next_task_schedule_time));

						gf_sleep((u32) diff);
						active_start = gf_sys_clock_high_res();
					}
					diff = (s64)task->schedule_next_time;
					diff -= (s64) gf_sys_clock_high_res();
					if (diff > 100 ) {
						Bool use_main = (current_filter->freg->flags & GF_FS_REG_MAIN_THREAD) ? GF_TRUE : GF_FALSE;
						GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: releasing current filter %s, exec time due in "LLD" us\n", sys_thid, current_filter->name, diff));
						current_filter->process_th_id = 0;
						current_filter->in_process = GF_FALSE;
						//don't touch the current filter tasks, just repost the task to the main/secondary list
						assert(gf_fq_count(current_filter->tasks));
						current_filter = NULL;

#ifdef CHECK_TASK_LIST_INTEGRITY
						check_task_list(fsess->main_thread_tasks, task);
						check_task_list(fsess->tasks, task);
						check_task_list(fsess->tasks_reservoir, task);
						assert(task->run_task);
#endif

						if (use_main) {
							gf_fq_add(fsess->main_thread_tasks, task);
							//we are the main thread and reposting to the main task list, don't notify/wait for the sema, just retry
							//we are sure to get a task from main list at next iteration
							if (use_main_sema) {
								skip_next_sema_wait = GF_TRUE;
							} else {
								gf_fs_sema_io(fsess, GF_TRUE, GF_TRUE);
							}
						} else {
							gf_fq_add(fsess->tasks, task);
							//we are not the main thread and we are reposting to the secondary task list, don't notify/wait for the sema, just retry
							//we are not sure to get a task from secondary list at next iteration, but the end of thread check will make
							//sure we renotify secondary sema if some tasks are still pending
							if (!use_main_sema) {
								skip_next_sema_wait = GF_TRUE;
							} else {
								gf_fs_sema_io(fsess, GF_TRUE, GF_FALSE);
							}
						}
						//we temporary force the main thread to fetch a task from the secondary list
						//because the first main task was not yet due for execution
						//it is likely that the execution of the next task will not wake up the main thread
						//but we must reevaluate the previous main task timing, so we force a notification of the main sema
						if (force_secondary_tasks)
							gf_fs_sema_io(fsess, GF_TRUE, GF_TRUE);

						continue;
					}
					force_secondary_tasks=GF_FALSE;
				}
			}
			GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u: task %s:%s schedule time "LLU" us reached (diff %d ms)\n", sys_thid, current_filter ? current_filter->name : "", task->log_name, task->schedule_next_time, (s32) diff));

		}
		next_task_schedule_time = 0;

		if (current_filter) {
			current_filter->scheduled_for_next_task = GF_TRUE;
			assert(!current_filter->in_process);
			current_filter->in_process = GF_TRUE;
			current_filter->process_th_id = gf_th_id();
		}

		sess_thread->nb_tasks++;
		sess_thread->has_seen_eot = GF_FALSE;
		if (task->filter) {
			assert(gf_fq_count(task->filter->tasks));
		}

		GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u task#%d %p executing Filter %s::%s (%d tasks pending, %d(%d) process task queued)\n", sys_thid, sess_thread->nb_tasks, task, task->filter ? task->filter->name : "none", task->log_name, fsess->tasks_pending, task->filter ? task->filter->process_task_queued : 0, task->filter ? gf_fq_count(task->filter->tasks) : 0));

		safe_int_inc(& fsess->tasks_in_process );
		assert( task->run_task );
		task_time = gf_sys_clock_high_res();
		//remember the last time we scheduled this filter
		if (task->filter)
			task->filter->last_schedule_task_time = task_time;

		task->can_swap = GF_FALSE;
		task->requeue_request = GF_FALSE;
		task->run_task(task);
		requeue = task->requeue_request;

		task_time = gf_sys_clock_high_res() - task_time;
		safe_int_dec(& fsess->tasks_in_process );

		//may now be NULL if task was a filter destruction task
		current_filter = task->filter;

#ifdef CHECK_TASK_LIST_INTEGRITY
		prev_current_filter = task->filter;
#endif

		//source task was current filter, pop the filter task list
		if (current_filter) {
			current_filter->nb_tasks_done++;
			current_filter->time_process += task_time;
			consecutive_filter_tasks++;

			gf_mx_p(current_filter->tasks_mx);
			//if last task
			if ( (gf_fq_count(current_filter->tasks)==1)
				//if requeue request and stream reset pending (we must exit the filter task loop for the reset task to pe processed)
				|| (requeue && current_filter->stream_reset_pending)
				//or requeue request and pid swap pending (we must exit the filter task loop for the swap task to pe processed)
				|| (requeue && (current_filter->swap_pidinst_src ||  current_filter->swap_pidinst_dst) )
				//or requeue request and pid detach / cap negotiate pending
				|| (requeue && (current_filter->out_pid_connection_pending || current_filter->detached_pid_inst || current_filter->caps_negociate) )

				//or requeue request and we have been running on that filter for more than 10 times, abort
				|| (requeue && (consecutive_filter_tasks>10))
			) {

				if (requeue) {
					//filter task can be pushed back the queue of tasks
					if (task->can_swap) {
						GF_FSTask *next_task;

						//drop task from filter task list
						gf_fq_pop(current_filter->tasks);

						next_task = gf_fq_head(current_filter->tasks);
						//if first task was notified, swap the flag
						if (next_task) {
							//see note in post_task_ex for caution about this !!
							next_task->notified = task->notified;
							task->notified = GF_FALSE;
						}
						//requeue task
						gf_fq_add(current_filter->tasks, task);

						//ans swap task for later requeing
						if (next_task) task = next_task;
					}
					//otherwise (can't swap) keep task first in the list

					//don't reset scheduled_for_next_task flag if requeued to make sure no other task posted from
					//another thread will post to main sched

#ifdef CHECK_TASK_LIST_INTEGRITY
					skip_filter_task_check = GF_TRUE;
#endif
				} else {
					//no requeue, filter no longer scheduled and drop task
					current_filter->scheduled_for_next_task = GF_FALSE;

					//drop task from filter task list
					gf_fq_pop(current_filter->tasks);
				}

				current_filter->process_th_id = 0;
				current_filter->in_process = GF_FALSE;

				//unlock once we modified in_process, otherwise this will make our assert fail
				gf_mx_v(current_filter->tasks_mx);
#ifdef CHECK_TASK_LIST_INTEGRITY
				if (requeue && !skip_filter_task_check) check_task_list(current_filter->tasks, task);
#endif
				current_filter = NULL;
			} else {
				//drop task from filter task list
				gf_fq_pop(current_filter->tasks);

				//not requeued, no more tasks, deactivate filter
				if (!requeue && !gf_fq_count(current_filter->tasks)) {
					current_filter->process_th_id = 0;
					current_filter->in_process = GF_FALSE;
					current_filter->scheduled_for_next_task = GF_FALSE;
					gf_mx_v(current_filter->tasks_mx);
					current_filter = NULL;
				} else {
#ifdef CHECK_TASK_LIST_INTEGRITY
					check_task_list(fsess->main_thread_tasks, task);
					check_task_list(fsess->tasks, task);
					check_task_list(fsess->tasks_reservoir, task);
#endif

					//requeue task in current filter
					if (requeue)
						gf_fq_add(current_filter->tasks, task);

					gf_mx_v(current_filter->tasks_mx);
				}
			}
		}
		//do not touch the filter task list after this, it has to be mutex protected to ensure proper posting of tasks

		notified = task->notified;
		if (requeue) {
			//if requeue on a filter active, use filter queue to avoid another thread grabing the task (we would have concurrent access to the filter)
			if (current_filter) {
				GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u re-posted task Filter %s::%s in filter tasks (%d pending)\n", sys_thid, task->filter->name, task->log_name, fsess->tasks_pending));
				task->notified = GF_FALSE;
				//keep this thread running on the current filter no signaling of semaphore
			} else {
				GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u re-posted task Filter %s::%s in %s tasks (%d pending)\n", sys_thid, task->filter ? task->filter->name : "none", task->log_name, (task->filter && (task->filter->freg->flags & GF_FS_REG_MAIN_THREAD)) ? "main" : "secondary", fsess->tasks_pending));

				task->notified = GF_TRUE;
				safe_int_inc(&fsess->tasks_pending);

#ifdef CHECK_TASK_LIST_INTEGRITY
				check_task_list(fsess->main_thread_tasks, task);
				check_task_list(fsess->tasks, task);
				check_task_list(fsess->tasks_reservoir, task);
				if (prev_current_filter && !skip_filter_task_check) check_task_list(prev_current_filter->tasks, task);
#endif

				//main thread
				if (task->filter && (task->filter->freg->flags & GF_FS_REG_MAIN_THREAD)) {
					gf_fq_add(fsess->main_thread_tasks, task);
				} else {
					gf_fq_add(fsess->tasks, task);
				}
				gf_fs_sema_io(fsess, GF_TRUE, use_main_sema);
			}
		} else {
#ifdef CHECK_TASK_LIST_INTEGRITY
			check_task_list(fsess->main_thread_tasks, task);
			check_task_list(fsess->tasks, task);
			check_task_list(fsess->tasks_reservoir, task);
			if (prev_current_filter)
				check_task_list(prev_current_filter->tasks, task);

			{
				gf_mx_p(fsess->filters_mx);
				u32 k, c2 = gf_list_count(fsess->filters);
				for (k=0; k<c2; k++) {
					GF_Filter *af = gf_list_get(fsess->filters, k);
					check_task_list(af->tasks, task);
				}
				gf_mx_v(fsess->filters_mx);
			}
#endif
			GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Thread %u task#%d %p pushed to reservoir\n", sys_thid, sess_thread->nb_tasks, task));

			if (fsess->tasks_reservoir) {
				memset(task, 0, sizeof(GF_FSTask));
				gf_fq_add(fsess->tasks_reservoir, task);
			} else {
				gf_free(task);
			}
		}

		//decrement task counter
		if (notified) {
			assert(fsess->tasks_pending);
			safe_int_dec(&fsess->tasks_pending);
		}
		if (current_filter) {
			current_filter->process_th_id = 0;
			current_filter->in_process = GF_FALSE;
		}
		//not requeuing and first time we have an empty task queue, flush to detect if we are indeed done
		if (!current_filter && !fsess->tasks_pending && !sess_thread->has_seen_eot && !gf_fq_count(fsess->tasks)) {
			//if not the main thread, or if main thread and task list is empty, enter end of session probing mode
			if (thid || !gf_fq_count(fsess->main_thread_tasks) ) {
				//maybe last task, force a notify to check if we are truly done. We only tag "session done" for the non-main
				//threads, in order to enter the end-of session signaling above
				if (thid) sess_thread->has_seen_eot = GF_TRUE;
				gf_fs_sema_io(fsess, GF_TRUE, use_main_sema);
			}
		}

		sess_thread->active_time += gf_sys_clock_high_res() - active_start;


		//no main thread, return
		if (!thid && fsess->no_main_thread && !current_filter && !fsess->pid_connect_tasks_pending) {
			gf_rmt_end();
			GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Main thread proc exit\n"));
			return 0;
		}
	}

	gf_rmt_end();

	//no main thread, return
	if (!thid && fsess->no_main_thread) {
		GF_LOG(GF_LOG_DEBUG, GF_LOG_SCHEDULER, ("Main thread proc exit\n"));
		return 0;
	}
	sess_thread->run_time = gf_sys_clock_high_res() - enter_time;

	safe_int_inc(&fsess->nb_threads_stopped);

	if (!fsess->run_status)
		fsess->run_status = GF_EOS;

	// thread exit, notify the semaphores
	if (fsess->semaphore_main && ! gf_sema_notify(fsess->semaphore_main, 1)) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_SCHEDULER, ("Failed to notify main semaphore, might hang up !!\n"));
	}
	if (fsess->semaphore_other && ! gf_sema_notify(fsess->semaphore_other, th_count)) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_SCHEDULER, ("Failed to notify secondary semaphore, might hang up !!\n"));
	}

	return 0;
}