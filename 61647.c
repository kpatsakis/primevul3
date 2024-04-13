_rpc_terminate_batch_job(uint32_t job_id, uint32_t user_id, char *node_name)
{
	int             rc     = SLURM_SUCCESS;
	int             nsteps = 0;
	int		delay;
	time_t		now = time(NULL);
	slurm_ctl_conf_t *cf;
	job_env_t job_env;

	task_g_slurmd_release_resources(job_id);

	if (_waiter_init(job_id) == SLURM_ERROR)
		return;

	/*
	 * "revoke" all future credentials for this jobid
	 */
	_note_batch_job_finished(job_id);
	if (slurm_cred_revoke(conf->vctx, job_id, now, now) < 0) {
		debug("revoking cred for job %u: %m", job_id);
	} else {
		save_cred_state(conf->vctx);
		debug("credential for job %u revoked", job_id);
	}

	/*
	 * Tasks might be stopped (possibly by a debugger)
	 * so send SIGCONT first.
	 */
	_kill_all_active_steps(job_id, SIGCONT, true);
	if (errno == ESLURMD_STEP_SUSPENDED) {
		/*
		 * If the job step is currently suspended, we don't
		 * bother with a "nice" termination.
		 */
		debug2("Job is currently suspended, terminating");
		nsteps = _terminate_all_steps(job_id, true);
	} else {
		nsteps = _kill_all_active_steps(job_id, SIGTERM, true);
	}

#ifndef HAVE_AIX
	if ((nsteps == 0) && !conf->epilog) {
		slurm_cred_begin_expiration(conf->vctx, job_id);
		save_cred_state(conf->vctx);
		_waiter_complete(job_id);
		if (container_g_delete(job_id))
			error("container_g_delete(%u): %m", job_id);
		_launch_complete_rm(job_id);
		return;
	}
#endif

	/*
	 *  Check for corpses
	 */
	cf = slurm_conf_lock();
	delay = MAX(cf->kill_wait, 5);
	slurm_conf_unlock();
	if (!_pause_for_job_completion(job_id, NULL, delay) &&
	     _terminate_all_steps(job_id, true) ) {
		/*
		 *  Block until all user processes are complete.
		 */
		_pause_for_job_completion(job_id, NULL, 0);
	}

	/*
	 *  Begin expiration period for cached information about job.
	 *   If expiration period has already begun, then do not run
	 *   the epilog again, as that script has already been executed
	 *   for this job.
	 */
	if (slurm_cred_begin_expiration(conf->vctx, job_id) < 0) {
		debug("Not running epilog for jobid %d: %m", job_id);
		goto done;
	}

	save_cred_state(conf->vctx);

	memset(&job_env, 0, sizeof(job_env_t));

	job_env.jobid = job_id;
	job_env.node_list = node_name;
	job_env.uid = (uid_t)user_id;
	/* NOTE: We lack the job's SPANK environment variables */
	rc = _run_epilog(&job_env);
	if (rc) {
		int term_sig, exit_status;
		if (WIFSIGNALED(rc)) {
			exit_status = 0;
			term_sig    = WTERMSIG(rc);
		} else {
			exit_status = WEXITSTATUS(rc);
			term_sig    = 0;
		}
		error("[job %u] epilog failed status=%d:%d",
		      job_id, exit_status, term_sig);
	} else
		debug("completed epilog for jobid %u", job_id);
	if (container_g_delete(job_id))
		error("container_g_delete(%u): %m", job_id);
	_launch_complete_rm(job_id);

    done:
	_wait_state_completed(job_id, 5);
	_waiter_complete(job_id);
}
