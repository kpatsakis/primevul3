_run_prolog(job_env_t *job_env, slurm_cred_t *cred)
{
	int rc;
	char *my_prolog;
	char **my_env;

	my_env = _build_env(job_env);
	setenvf(&my_env, "SLURM_STEP_ID", "%u", job_env->step_id);

	slurm_mutex_lock(&conf->config_mutex);
	my_prolog = xstrdup(conf->prolog);
	slurm_mutex_unlock(&conf->config_mutex);

	rc = _run_job_script("prolog", my_prolog, job_env->jobid,
			     -1, my_env, job_env->uid);
	_remove_job_running_prolog(job_env->jobid);
	xfree(my_prolog);
	_destroy_env(my_env);

	return rc;
}
