GF_FilterSession *gf_fs_new_defaults(u32 inflags)
{
	GF_FilterSession *fsess;
	GF_FilterSchedulerType sched_type = GF_FS_SCHEDULER_LOCK_FREE;
	u32 flags = 0;
	s32 nb_threads = gf_opts_get_int("core", "threads");
	const char *blacklist = gf_opts_get_key("core", "blacklist");
	const char *opt = gf_opts_get_key("core", "sched");

	if (!opt) sched_type = GF_FS_SCHEDULER_LOCK_FREE;
	else if (!strcmp(opt, "lock")) sched_type = GF_FS_SCHEDULER_LOCK;
	else if (!strcmp(opt, "flock")) sched_type = GF_FS_SCHEDULER_LOCK_FORCE;
	else if (!strcmp(opt, "direct")) sched_type = GF_FS_SCHEDULER_DIRECT;
	else if (!strcmp(opt, "free")) sched_type = GF_FS_SCHEDULER_LOCK_FREE;
	else if (!strcmp(opt, "freex")) sched_type = GF_FS_SCHEDULER_LOCK_FREE_X;
	else {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Unrecognized scheduler type %s\n", opt));
		return NULL;
	}
	if (inflags & GF_FS_FLAG_LOAD_META)
		flags |= GF_FS_FLAG_LOAD_META;

	if (inflags & GF_FS_FLAG_NO_MAIN_THREAD)
		flags |= GF_FS_FLAG_NO_MAIN_THREAD;

	if (inflags & GF_FS_FLAG_NO_GRAPH_CACHE)
		flags |= GF_FS_FLAG_NO_GRAPH_CACHE;

	if (gf_opts_get_bool("core", "dbg-edges"))
		flags |= GF_FS_FLAG_PRINT_CONNECTIONS;

	if (gf_opts_get_bool("core", "full-link"))
		flags |= GF_FS_FLAG_FULL_LINK;

	if (gf_opts_get_bool("core", "no-reg"))
		flags |= GF_FS_FLAG_NO_REGULATION;

	if (gf_opts_get_bool("core", "no-reassign"))
		flags |= GF_FS_FLAG_NO_REASSIGN;

	if (gf_opts_get_bool("core", "no-graph-cache"))
		flags |= GF_FS_FLAG_NO_GRAPH_CACHE;

	if (gf_opts_get_bool("core", "no-probe"))
		flags |= GF_FS_FLAG_NO_PROBE;
	else if (inflags & GF_FS_FLAG_NO_PROBE)
		flags |= GF_FS_FLAG_NO_PROBE;

	if (gf_opts_get_bool("core", "no-argchk"))
		flags |= GF_FS_FLAG_NO_ARG_CHECK;

	if (gf_opts_get_bool("core", "no-reservoir"))
		flags |= GF_FS_FLAG_NO_RESERVOIR;


	fsess = gf_fs_new(nb_threads, sched_type, flags, blacklist);
	if (!fsess) return NULL;

	gf_fs_set_max_resolution_chain_length(fsess, gf_opts_get_int("core", "max-chain") );

	gf_fs_set_max_sleep_time(fsess, gf_opts_get_int("core", "max-sleep") );

	opt = gf_opts_get_key("core", "seps");
	if (opt)
		gf_fs_set_separators(fsess, opt);

	return fsess;
}