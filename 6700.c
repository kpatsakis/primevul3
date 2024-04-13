GF_Err gf_fs_stop(GF_FilterSession *fsess)
{
	u32 i, count = fsess->threads ? gf_list_count(fsess->threads) : 0;

	GF_LOG(GF_LOG_DEBUG, GF_LOG_FILTER, ("Session stop\n"));
	if (count+1 == fsess->nb_threads_stopped) {
		return GF_OK;
	}

	if (!fsess->run_status) {
		fsess->in_final_flush = GF_TRUE;
		fsess->run_status = GF_EOS;
	}

	for (i=0; i < count; i++) {
		gf_fs_sema_io(fsess, GF_TRUE, GF_FALSE);
	}

	//wait for all threads to be done, we might still need flushing the main thread queue
	while (fsess->no_main_thread) {
		gf_fs_thread_proc(&fsess->main_th);
		if (gf_fq_count(fsess->main_thread_tasks))
			continue;

		if (count && (count == fsess->nb_threads_stopped) && gf_fq_count(fsess->tasks) ) {
			continue;
		}
		break;
	}
	if (fsess->no_main_thread) {
		safe_int_inc(&fsess->nb_threads_stopped);
		fsess->main_th.has_seen_eot = GF_TRUE;
	}

	while (count+1 != fsess->nb_threads_stopped) {
		for (i=0; i < count; i++) {
			gf_fs_sema_io(fsess, GF_TRUE, GF_FALSE);
		}
		gf_sleep(0);
		//we may have tasks in main task list posted by other threads
		if (fsess->no_main_thread) {
			gf_fs_thread_proc(&fsess->main_th);
			fsess->main_th.has_seen_eot = GF_TRUE;
		}
	}
	return GF_OK;
}