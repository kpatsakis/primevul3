_run_prolog(job_env_t *job_env, slurm_cred_t *cred)
{
	DEF_TIMERS;
	int rc, diff_time;
	char *my_prolog;
	time_t start_time = time(NULL);
	static uint16_t msg_timeout = 0;
	static uint16_t timeout;
	pthread_t       timer_id;
	pthread_attr_t  timer_attr;
	pthread_cond_t  timer_cond  = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;
	timer_struct_t  timer_struct;
	bool prolog_fini = false;
	char **my_env;

	my_env = _build_env(job_env);
	setenvf(&my_env, "SLURM_STEP_ID", "%u", job_env->step_id);
	if (cred) {
		slurm_cred_arg_t cred_arg;
		slurm_cred_get_args(cred, &cred_arg);
		setenvf(&my_env, "SLURM_JOB_CONSTRAINTS", "%s",
			cred_arg.job_constraints);
		gres_plugin_job_set_env(&my_env, cred_arg.job_gres_list);
		slurm_cred_free_args(&cred_arg);
	}

	if (msg_timeout == 0)
		msg_timeout = slurm_get_msg_timeout();

	if (timeout == 0)
		timeout = slurm_get_prolog_timeout();

	slurm_mutex_lock(&conf->config_mutex);
	my_prolog = xstrdup(conf->prolog);
	slurm_mutex_unlock(&conf->config_mutex);

	slurm_attr_init(&timer_attr);
	timer_struct.job_id      = job_env->jobid;
	timer_struct.msg_timeout = msg_timeout;
	timer_struct.prolog_fini = &prolog_fini;
	timer_struct.timer_cond  = &timer_cond;
	timer_struct.timer_mutex = &timer_mutex;
	pthread_create(&timer_id, &timer_attr, &_prolog_timer, &timer_struct);
	START_TIMER;

	if (timeout == (uint16_t)NO_VAL)
		rc = _run_job_script("prolog", my_prolog, job_env->jobid,
				     -1, my_env, job_env->uid);
	else
		rc = _run_job_script("prolog", my_prolog, job_env->jobid,
				     timeout, my_env, job_env->uid);

	END_TIMER;
	info("%s: run job script took %s", __func__, TIME_STR);
	slurm_mutex_lock(&timer_mutex);
	prolog_fini = true;
	pthread_cond_broadcast(&timer_cond);
	slurm_mutex_unlock(&timer_mutex);

	diff_time = difftime(time(NULL), start_time);
	info("%s: prolog with lock for job %u ran for %d seconds",
	     __func__, job_env->jobid, diff_time);
	if (diff_time >= (msg_timeout / 2)) {
		info("prolog for job %u ran for %d seconds",
		     job_env->jobid, diff_time);
	}

	_remove_job_running_prolog(job_env->jobid);
	xfree(my_prolog);
	_destroy_env(my_env);

	pthread_join(timer_id, NULL);
	return rc;
}
