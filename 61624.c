_requeue_setup_env_fail(void)
{
	static time_t config_update = 0;
	static bool requeue = false;

	if (config_update != conf->last_update) {
		char *sched_params = slurm_get_sched_params();
		requeue = (sched_params &&
			   (strstr(sched_params, "no_env_cache") ||
			    strstr(sched_params, "requeue_setup_env_fail")));
		xfree(sched_params);
		config_update = conf->last_update;
	}

	return requeue;
}
