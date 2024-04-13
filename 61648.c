_rpc_terminate_job(slurm_msg_t *msg)
{
#ifndef HAVE_AIX
	bool		have_spank = false;
#endif
	int             rc     = SLURM_SUCCESS;
	kill_job_msg_t *req    = msg->data;
	uid_t           uid    = g_slurm_auth_get_uid(msg->auth_cred,
						      conf->auth_info);
	int             nsteps = 0;
	int		delay;
	job_env_t       job_env;

	debug("_rpc_terminate_job, uid = %d", uid);
	/*
	 * check that requesting user ID is the SLURM UID
	 */
	if (!_slurm_authorized_user(uid)) {
		error("Security violation: kill_job(%u) from uid %d",
		      req->job_id, uid);
		if (msg->conn_fd >= 0)
			slurm_send_rc_msg(msg, ESLURM_USER_ID_MISSING);
		return;
	}

	task_g_slurmd_release_resources(req->job_id);

	/*
	 *  Initialize a "waiter" thread for this jobid. If another
	 *   thread is already waiting on termination of this job,
	 *   _waiter_init() will return SLURM_ERROR. In this case, just
	 *   notify slurmctld that we recvd the message successfully,
	 *   then exit this thread.
	 */
	if (_waiter_init(req->job_id) == SLURM_ERROR) {
		if (msg->conn_fd >= 0) {
			/* No matter if the step hasn't started yet or
			 * not just send a success to let the
			 * controller know we got this request.
			 */
			slurm_send_rc_msg (msg, SLURM_SUCCESS);
		}
		return;
	}

	/*
	 * "revoke" all future credentials for this jobid
	 */
	if (slurm_cred_revoke(conf->vctx, req->job_id, req->time,
			      req->start_time) < 0) {
		debug("revoking cred for job %u: %m", req->job_id);
	} else {
		save_cred_state(conf->vctx);
		debug("credential for job %u revoked", req->job_id);
	}

	/*
	 * Before signalling steps, if the job has any steps that are still
	 * in the process of fork/exec/check in with slurmd, wait on a condition
	 * var for the start.  Otherwise a slow-starting step can miss the
	 * job termination message and run indefinitely.
	 */
	if (_step_is_starting(req->job_id, NO_VAL)) {
		if (msg->conn_fd >= 0) {
			/* If the step hasn't started yet just send a
			 * success to let the controller know we got
			 * this request.
			 */
			debug("sent SUCCESS, waiting for step to start");
			slurm_send_rc_msg (msg, SLURM_SUCCESS);
			if (slurm_close(msg->conn_fd) < 0)
				error ( "rpc_kill_job: close(%d): %m",
					msg->conn_fd);
			msg->conn_fd = -1;
		}
		if (_wait_for_starting_step(req->job_id, NO_VAL)) {
			/*
			 * There's currently no case in which we enter this
			 * error condition.  If there was, it's hard to say
			 * whether to to proceed with the job termination.
			 */
			error("Error in _wait_for_starting_step");
		}
	}
	if (IS_JOB_NODE_FAILED(req))
		_kill_all_active_steps(req->job_id, SIG_NODE_FAIL, true);
	if (IS_JOB_PENDING(req))
		_kill_all_active_steps(req->job_id, SIG_REQUEUED, true);
	else if (IS_JOB_FAILED(req))
		_kill_all_active_steps(req->job_id, SIG_FAILURE, true);

	/*
	 * Tasks might be stopped (possibly by a debugger)
	 * so send SIGCONT first.
	 */
	_kill_all_active_steps(req->job_id, SIGCONT, true);
	if (errno == ESLURMD_STEP_SUSPENDED) {
		/*
		 * If the job step is currently suspended, we don't
		 * bother with a "nice" termination.
		 */
		debug2("Job is currently suspended, terminating");
		nsteps = _terminate_all_steps(req->job_id, true);
	} else {
		nsteps = _kill_all_active_steps(req->job_id, SIGTERM, true);
	}

#ifndef HAVE_AIX
	if ((nsteps == 0) && !conf->epilog) {
		struct stat stat_buf;
		if (conf->plugstack && (stat(conf->plugstack, &stat_buf) == 0))
			have_spank = true;
	}
	/*
	 *  If there are currently no active job steps and no
	 *    configured epilog to run, bypass asynchronous reply and
	 *    notify slurmctld that we have already completed this
	 *    request. We need to send current switch state on AIX
	 *    systems, so this bypass can not be used.
	 */
	if ((nsteps == 0) && !conf->epilog && !have_spank) {
		debug4("sent ALREADY_COMPLETE");
		if (msg->conn_fd >= 0) {
			slurm_send_rc_msg(msg,
					  ESLURMD_KILL_JOB_ALREADY_COMPLETE);
		}
		slurm_cred_begin_expiration(conf->vctx, req->job_id);
		save_cred_state(conf->vctx);
		_waiter_complete(req->job_id);

		/*
		 * The controller needs to get MESSAGE_EPILOG_COMPLETE to bring
		 * the job out of "completing" state.  Otherwise, the job
		 * could remain "completing" unnecessarily, until the request
		 * to terminate is resent.
		 */
		_sync_messages_kill(req);
		if (msg->conn_fd < 0) {
			/* The epilog complete message processing on
			 * slurmctld is equivalent to that of a
			 * ESLURMD_KILL_JOB_ALREADY_COMPLETE reply above */
			_epilog_complete(req->job_id, rc);
		}
		if (container_g_delete(req->job_id))
			error("container_g_delete(%u): %m", req->job_id);
		_launch_complete_rm(req->job_id);
		return;
	}
#endif

	/*
	 *  At this point, if connection still open, we send controller
	 *   a "success" reply to indicate that we've recvd the msg.
	 */
	if (msg->conn_fd >= 0) {
		debug4("sent SUCCESS");
		slurm_send_rc_msg(msg, SLURM_SUCCESS);
		if (slurm_close(msg->conn_fd) < 0)
			error ("rpc_kill_job: close(%d): %m", msg->conn_fd);
		msg->conn_fd = -1;
	}

	/*
	 *  Check for corpses
	 */
	delay = MAX(conf->kill_wait, 5);
	if ( !_pause_for_job_completion (req->job_id, req->nodes, delay) &&
	     _terminate_all_steps(req->job_id, true) ) {
		/*
		 *  Block until all user processes are complete.
		 */
		_pause_for_job_completion (req->job_id, req->nodes, 0);
	}

	/*
	 *  Begin expiration period for cached information about job.
	 *   If expiration period has already begun, then do not run
	 *   the epilog again, as that script has already been executed
	 *   for this job.
	 */
	if (slurm_cred_begin_expiration(conf->vctx, req->job_id) < 0) {
		debug("Not running epilog for jobid %d: %m", req->job_id);
		goto done;
	}

	save_cred_state(conf->vctx);

	memset(&job_env, 0, sizeof(job_env_t));

	job_env.jobid = req->job_id;
	job_env.node_list = req->nodes;
	job_env.spank_job_env = req->spank_job_env;
	job_env.spank_job_env_size = req->spank_job_env_size;
	job_env.uid = req->job_uid;

#if defined(HAVE_BG)
	select_g_select_jobinfo_get(req->select_jobinfo,
				    SELECT_JOBDATA_BLOCK_ID,
				    &job_env.resv_id);
#elif defined(HAVE_ALPS_CRAY)
	job_env.resv_id = select_g_select_jobinfo_xstrdup(
		req->select_jobinfo, SELECT_PRINT_RESV_ID);
#endif
	rc = _run_epilog(&job_env);
	xfree(job_env.resv_id);

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
		      req->job_id, exit_status, term_sig);
		rc = ESLURMD_EPILOG_FAILED;
	} else
		debug("completed epilog for jobid %u", req->job_id);
	if (container_g_delete(req->job_id))
		error("container_g_delete(%u): %m", req->job_id);
	_launch_complete_rm(req->job_id);

    done:
	_wait_state_completed(req->job_id, 5);
	_waiter_complete(req->job_id);
	_sync_messages_kill(req);

	_epilog_complete(req->job_id, rc);
}
