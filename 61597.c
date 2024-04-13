_get_user_env(batch_job_launch_msg_t *req)
{
	struct passwd pwd, *pwd_ptr = NULL;
	char pwd_buf[PW_BUF_SIZE];
	char **new_env;
	int i;
	static time_t config_update = 0;
	static bool no_env_cache = false;

	if (config_update != conf->last_update) {
		char *sched_params = slurm_get_sched_params();
		no_env_cache = (sched_params &&
				strstr(sched_params, "no_env_cache"));
		xfree(sched_params);
		config_update = conf->last_update;
	}

	for (i=0; i<req->envc; i++) {
		if (xstrcmp(req->environment[i], "SLURM_GET_USER_ENV=1") == 0)
			break;
	}
	if (i >= req->envc)
		return 0;		/* don't need to load env */

	if (slurm_getpwuid_r(req->uid, &pwd, pwd_buf, PW_BUF_SIZE, &pwd_ptr)
	    || (pwd_ptr == NULL)) {
		error("%s: getpwuid_r(%u):%m", __func__, req->uid);
		return -1;
	}
	verbose("%s: get env for user %s here", __func__, pwd.pw_name);

	/* Permit up to 120 second delay before using cache file */
	new_env = env_array_user_default(pwd.pw_name, 120, 0, no_env_cache);
	if (! new_env) {
		error("%s: Unable to get user's local environment%s",
		      __func__, no_env_cache ?
		      "" : ", running only with passed environment");
		return -1;
	}

	env_array_merge(&new_env,
			(const char **) req->environment);
	env_array_free(req->environment);
	req->environment = new_env;
	req->envc = envcount(new_env);

	return 0;
}
