_rpc_signal_tasks(slurm_msg_t *msg)
{
	int               rc = SLURM_SUCCESS;
	uid_t             req_uid = g_slurm_auth_get_uid(msg->auth_cred,
							 conf->auth_info);
	kill_tasks_msg_t *req = (kill_tasks_msg_t *) msg->data;
	uint32_t flag;
	uint32_t sig;

	flag = req->signal >> 24;
	sig  = req->signal & 0xfff;

	if (flag & KILL_FULL_JOB) {
		debug("%s: sending signal %u to entire job %u flag %u",
		      __func__, sig, req->job_id, flag);
		_kill_all_active_steps(req->job_id, sig, true);
	} else if (flag & KILL_STEPS_ONLY) {
		debug("%s: sending signal %u to all steps job %u flag %u",
		      __func__, sig, req->job_id, flag);
		_kill_all_active_steps(req->job_id, sig, false);
	} else {
		debug("%s: sending signal %u to step %u.%u flag %u", __func__,
		      sig, req->job_id, req->job_step_id, flag);
		rc = _signal_jobstep(req->job_id, req->job_step_id, req_uid,
				     req->signal);
	}
	slurm_send_rc_msg(msg, rc);
}
