_rpc_stat_jobacct(slurm_msg_t *msg)
{
	job_step_id_msg_t *req = (job_step_id_msg_t *)msg->data;
	slurm_msg_t        resp_msg;
	job_step_stat_t *resp = NULL;
	int fd;
	uid_t req_uid, uid;
	uint16_t protocol_version;

	debug3("Entering _rpc_stat_jobacct");
	/* step completion messages are only allowed from other slurmstepd,
	   so only root or SlurmUser is allowed here */
	req_uid = g_slurm_auth_get_uid(msg->auth_cred, conf->auth_info);

	fd = stepd_connect(conf->spooldir, conf->node_name,
			   req->job_id, req->step_id, &protocol_version);
	if (fd == -1) {
		error("stepd_connect to %u.%u failed: %m",
		      req->job_id, req->step_id);
		slurm_send_rc_msg(msg, ESLURM_INVALID_JOB_ID);
		return	ESLURM_INVALID_JOB_ID;
	}

	if ((int)(uid = stepd_get_uid(fd, protocol_version)) < 0) {
		debug("stat_jobacct couldn't read from the step %u.%u: %m",
		      req->job_id, req->step_id);
		close(fd);
		if (msg->conn_fd >= 0)
			slurm_send_rc_msg(msg, ESLURM_INVALID_JOB_ID);
		return	ESLURM_INVALID_JOB_ID;
	}

	/*
	 * check that requesting user ID is the SLURM UID or root
	 */
	if ((req_uid != uid) && (!_slurm_authorized_user(req_uid))) {
		error("stat_jobacct from uid %ld for job %u "
		      "owned by uid %ld",
		      (long) req_uid, req->job_id, (long) uid);

		if (msg->conn_fd >= 0) {
			slurm_send_rc_msg(msg, ESLURM_USER_ID_MISSING);
			close(fd);
			return ESLURM_USER_ID_MISSING;/* or bad in this case */
		}
	}

	resp = xmalloc(sizeof(job_step_stat_t));
	resp->step_pids = xmalloc(sizeof(job_step_pids_t));
	resp->step_pids->node_name = xstrdup(conf->node_name);
	slurm_msg_t_copy(&resp_msg, msg);
	resp->return_code = SLURM_SUCCESS;

	if (stepd_stat_jobacct(fd, protocol_version, req, resp)
	    == SLURM_ERROR) {
		debug("accounting for nonexistent job %u.%u requested",
		      req->job_id, req->step_id);
	}

	/* FIX ME: This should probably happen in the
	   stepd_stat_jobacct to get more information about the pids.
	*/
	if (stepd_list_pids(fd, protocol_version, &resp->step_pids->pid,
			    &resp->step_pids->pid_cnt) == SLURM_ERROR) {
		debug("No pids for nonexistent job %u.%u requested",
		      req->job_id, req->step_id);
	}

	close(fd);

	resp_msg.msg_type     = RESPONSE_JOB_STEP_STAT;
	resp_msg.data         = resp;

	slurm_send_node_msg(msg->conn_fd, &resp_msg);
	slurm_free_job_step_stat(resp);
	return SLURM_SUCCESS;
}
