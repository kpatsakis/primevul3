_rpc_timelimit(slurm_msg_t *msg)
{
	uid_t           uid = g_slurm_auth_get_uid(msg->auth_cred,
						   conf->auth_info);
	kill_job_msg_t *req = msg->data;
	int             nsteps, rc;

	if (!_slurm_authorized_user(uid)) {
		error ("Security violation: rpc_timelimit req from uid %d",
		       uid);
		slurm_send_rc_msg(msg, ESLURM_USER_ID_MISSING);
		return;
	}

	/*
	 *  Indicate to slurmctld that we've received the message
	 */
	slurm_send_rc_msg(msg, SLURM_SUCCESS);
	slurm_close(msg->conn_fd);
	msg->conn_fd = -1;

	if (req->step_id != NO_VAL) {
		slurm_ctl_conf_t *cf;
		int delay;
		/* A jobstep has timed out:
		 * - send the container a SIG_TIME_LIMIT or SIG_PREEMPTED
		 *   to log the event
		 * - send a SIGCONT to resume any suspended tasks
		 * - send a SIGTERM to begin termination
		 * - sleep KILL_WAIT
		 * - send a SIGKILL to clean up
		 */
		if (msg->msg_type == REQUEST_KILL_TIMELIMIT) {
			rc = _signal_jobstep(req->job_id, req->step_id, uid,
					     SIG_TIME_LIMIT);
		} else {
			rc = _signal_jobstep(req->job_id, req->step_id, uid,
					     SIG_PREEMPTED);
		}
		if (rc != SLURM_SUCCESS)
			return;
		rc = _signal_jobstep(req->job_id, req->step_id, uid, SIGCONT);
		if (rc != SLURM_SUCCESS)
			return;
		rc = _signal_jobstep(req->job_id, req->step_id, uid, SIGTERM);
		if (rc != SLURM_SUCCESS)
			return;
		cf = slurm_conf_lock();
		delay = MAX(cf->kill_wait, 5);
		slurm_conf_unlock();
		sleep(delay);
		_signal_jobstep(req->job_id, req->step_id, uid, SIGKILL);
		return;
	}

	if (msg->msg_type == REQUEST_KILL_TIMELIMIT)
		_kill_all_active_steps(req->job_id, SIG_TIME_LIMIT, true);
	else /* (msg->type == REQUEST_KILL_PREEMPTED) */
		_kill_all_active_steps(req->job_id, SIG_PREEMPTED, true);
	nsteps = _kill_all_active_steps(req->job_id, SIGTERM, false);
	verbose( "Job %u: timeout: sent SIGTERM to %d active steps",
		 req->job_id, nsteps );

	/* Revoke credential, send SIGKILL, run epilog, etc. */
	_rpc_terminate_job(msg);
}
