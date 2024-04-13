GF_FilterSession *gf_fs_new(s32 nb_threads, GF_FilterSchedulerType sched_type, u32 flags, const char *blacklist)
{
	const char *opt;
	Bool gf_sys_has_filter_global_args();
	Bool gf_sys_has_filter_global_meta_args();

	u32 i, count;
	GF_FilterSession *fsess, *a_sess;

	//safety check: all built-in properties shall have unique 4CCs
	if ( ! gf_props_4cc_check_props())
		return NULL;

	GF_SAFEALLOC(fsess, GF_FilterSession);
	if (!fsess) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Failed to alloc media session\n"));
		return NULL;
	}
	fsess->flags = flags;

	fsess->filters = gf_list_new();
	fsess->main_th.fsess = fsess;

	if ((s32) nb_threads == -1) {
		GF_SystemRTInfo rti;
		memset(&rti, 0, sizeof(GF_SystemRTInfo));
		if (gf_sys_get_rti(0, &rti, 0)) {
			nb_threads = rti.nb_cores-1;
		}
		if ((s32)nb_threads<0) {
			GF_LOG(GF_LOG_WARNING, GF_LOG_FILTER, ("Failed to query number of cores, disabling extra threads for session\n"));
			nb_threads=0;
		}
	}

	if (sched_type==GF_FS_SCHEDULER_DIRECT) {
		fsess->direct_mode = GF_TRUE;
		nb_threads=0;
	}
	if (nb_threads && (sched_type != GF_FS_SCHEDULER_LOCK_FREE_X)) {
		fsess->tasks_mx = gf_mx_new("TasksList");
	}

	//regardless of scheduler type, we don't use lock on the main task list
	fsess->tasks = gf_fq_new(fsess->tasks_mx);

	if (nb_threads>0) {
		fsess->main_thread_tasks = gf_fq_new(fsess->tasks_mx);
		fsess->filters_mx = gf_mx_new("Filters");
	} else {
		//otherwise use the same as the global task list
		fsess->main_thread_tasks = fsess->tasks;
	}

	if (!(flags & GF_FS_FLAG_NO_RESERVOIR)) {
		fsess->tasks_reservoir = gf_fq_new(fsess->tasks_mx);
	}

	if (nb_threads || (sched_type==GF_FS_SCHEDULER_LOCK_FORCE) ) {
		fsess->semaphore_main = fsess->semaphore_other = gf_sema_new(GF_INT_MAX, 0);
		if (nb_threads>0)
			fsess->semaphore_other = gf_sema_new(GF_INT_MAX, 0);

		//force testing of mutex queues
		//fsess->use_locks = GF_TRUE;
	}
	fsess->ui_event_proc = fs_default_event_proc;
	fsess->ui_opaque = fsess;

	if (flags & GF_FS_FLAG_NO_MAIN_THREAD)
		fsess->no_main_thread = GF_TRUE;

	if (!fsess->semaphore_main)
		nb_threads=0;

	if (nb_threads) {
		fsess->threads = gf_list_new();
		if (!fsess->threads) {
			gf_sema_del(fsess->semaphore_main);
			fsess->semaphore_main=NULL;
			gf_sema_del(fsess->semaphore_other);
			fsess->semaphore_other=NULL;
			nb_threads=0;
		}
		fsess->use_locks = (sched_type==GF_FS_SCHEDULER_LOCK) ? GF_TRUE : GF_FALSE;
	} else {
#ifdef GPAC_MEMORY_TRACKING
		extern int gf_mem_track_enabled;
		fsess->check_allocs = gf_mem_track_enabled;
#endif

	}

	if (fsess->use_locks)
		fsess->props_mx = gf_mx_new("FilterSessionProps");

	if (!(flags & GF_FS_FLAG_NO_RESERVOIR)) {
#if GF_PROPS_HASHTABLE_SIZE
		fsess->prop_maps_list_reservoir = gf_fq_new(fsess->props_mx);
#endif
		fsess->prop_maps_reservoir = gf_fq_new(fsess->props_mx);
		fsess->prop_maps_entry_reservoir = gf_fq_new(fsess->props_mx);
		fsess->prop_maps_entry_data_alloc_reservoir = gf_fq_new(fsess->props_mx);
		//we also use the props mutex for the this one
		fsess->pcks_refprops_reservoir = gf_fq_new(fsess->props_mx);
	}


#ifndef GPAC_DISABLE_REMOTERY
	sprintf(fsess->main_th.rmt_name, "FSThread0");
#endif

	if (!fsess->filters || !fsess->tasks) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Failed to alloc media session\n"));
		fsess->run_status = GF_OUT_OF_MEM;
		gf_fs_del(fsess);
		return NULL;
	}

	if (nb_threads) {
		fsess->info_mx = gf_mx_new("FilterSessionInfo");
		fsess->ui_mx = gf_mx_new("FilterSessionUIProc");
	}

	for (i=0; i<(u32) nb_threads; i++) {
		char szName[30];
		GF_SessionThread *sess_thread;
		GF_SAFEALLOC(sess_thread, GF_SessionThread);
		if (!sess_thread) continue;
#ifndef GPAC_DISABLE_REMOTERY
		sprintf(sess_thread->rmt_name, "FSThread%d", i+1);
#endif
		sprintf(szName, "gf_fs_th_%d", i+1);
		sess_thread->th = gf_th_new(szName);
		if (!sess_thread->th) {
			gf_free(sess_thread);
			continue;
		}
		sess_thread->fsess = fsess;
		gf_list_add(fsess->threads, sess_thread);
	}

	gf_fs_set_separators(fsess, NULL);

	fsess->registry = gf_list_new();
	fsess->blacklist = blacklist;
	a_sess = (flags & GF_FS_FLAG_LOAD_META) ? fsess : NULL;
	gf_fs_reg_all(fsess, a_sess);

	//load external modules
	count = gf_modules_count();
	for (i=0; i<count; i++) {
		GF_FilterRegister *freg = (GF_FilterRegister *) gf_modules_load_filter(i, a_sess);
		if (freg) {
			gf_fs_add_filter_register(fsess, freg);
		}
	}
	fsess->blacklist = NULL;

	//todo - find a way to handle events without mutex ...
	fsess->evt_mx = gf_mx_new("Event mutex");

	fsess->blocking_mode = GF_FS_BLOCK_ALL;
	opt = gf_opts_get_key("core", "no-block");
	if (opt) {
		if (!strcmp(opt, "fanout")) {
			fsess->blocking_mode = GF_FS_NOBLOCK_FANOUT;
		}
		else if (!strcmp(opt, "all")) {
			fsess->blocking_mode = GF_FS_NOBLOCK;
		}
	}

	fsess->run_status = GF_EOS;
	fsess->nb_threads_stopped = 1+nb_threads;
	fsess->default_pid_buffer_max_us = 1000;
	fsess->decoder_pid_buffer_max_us = 1000000;
	fsess->default_pid_buffer_max_units = 1;
	fsess->max_resolve_chain_len = 6;
	fsess->auto_inc_nums = gf_list_new();

	if (nb_threads)
		fsess->links_mx = gf_mx_new("FilterRegistryGraph");
	fsess->links = gf_list_new();

#ifndef GPAC_DISABLE_3D
	fsess->gl_providers = gf_list_new();
#endif

	if (! (fsess->flags & GF_FS_FLAG_NO_GRAPH_CACHE))
		gf_filter_sess_build_graph(fsess, NULL);

	fsess->init_done = GF_TRUE;


	if (gf_sys_has_filter_global_args() || gf_sys_has_filter_global_meta_args()) {
		u32 nb_args = gf_sys_get_argc();
		for (i=0; i<nb_args; i++) {
			char *arg = (char *)gf_sys_get_arg(i);
			if (arg[0]!='-') continue;
			if ((arg[1]!='-') && (arg[1]!='+')) continue;
			char *sep = strchr(arg, '=');
			if (sep) sep[0] = 0;
			gf_fs_push_arg(fsess, arg+2, 0, (arg[1]!='-') ? 2 : 1);

			if (sep && !strcmp(arg+2, "template") && strstr(sep+1, "$Bandwidth$")) {
				gf_opts_set_key("temp", "force_indexing", "true");
			}

			if (sep) sep[0] = '=';
		}
	}

#ifdef GF_FS_ENABLE_LOCALES
	fsess->uri_relocators = gf_list_new();
	fsess->locales.relocate_uri = fs_check_locales;
	fsess->locales.sess = fsess;
	gf_list_add(fsess->uri_relocators, &fsess->locales);
#endif
	return fsess;
}