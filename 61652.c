_run_epilog(job_env_t *job_env)
{
	time_t start_time = time(NULL);
	static uint16_t msg_timeout = 0;
	static uint16_t timeout;
	int error_code, diff_time;
	char *my_epilog;
	char **my_env = _build_env(job_env);

	if (msg_timeout == 0)
		msg_timeout = slurm_get_msg_timeout();

	if (timeout == 0)
		timeout = slurm_get_prolog_timeout();

	slurm_mutex_lock(&conf->config_mutex);
	my_epilog = xstrdup(conf->epilog);
	slurm_mutex_unlock(&conf->config_mutex);

	_wait_for_job_running_prolog(job_env->jobid);

	if (timeout == (uint16_t)NO_VAL)
		error_code = _run_job_script("epilog", my_epilog, job_env->jobid,
					     -1, my_env, job_env->uid);
	else
		error_code = _run_job_script("epilog", my_epilog, job_env->jobid,
					     timeout, my_env, job_env->uid);

	xfree(my_epilog);
	_destroy_env(my_env);

	diff_time = difftime(time(NULL), start_time);
	if (diff_time >= (msg_timeout / 2)) {
		info("epilog for job %u ran for %d seconds",
		     job_env->jobid, diff_time);
	}

	return error_code;
}
