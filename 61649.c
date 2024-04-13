_rpc_terminate_tasks(slurm_msg_t *msg)
{
	kill_tasks_msg_t *req = (kill_tasks_msg_t *) msg->data;
	int               rc = SLURM_SUCCESS;
	int               fd;
	uid_t             req_uid, uid;
	uint16_t protocol_version;

	debug3("Entering _rpc_terminate_tasks");
	fd = stepd_connect(conf->spooldir, conf->node_name,
			   req->job_id, req->job_step_id, &protocol_version);
	if (fd == -1) {
		debug("kill for nonexistent job %u.%u stepd_connect "
		      "failed: %m", req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done;
	}

	if ((int)(uid = stepd_get_uid(fd, protocol_version)) < 0) {
		debug("terminate_tasks couldn't read from the step %u.%u: %m",
		      req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done2;
	}

	req_uid = g_slurm_auth_get_uid(msg->auth_cred, conf->auth_info);
	if ((req_uid != uid)
	    && (!_slurm_authorized_user(req_uid))) {
		debug("kill req from uid %ld for job %u.%u owned by uid %ld",
		      (long) req_uid, req->job_id, req->job_step_id,
		      (long) uid);
		rc = ESLURM_USER_ID_MISSING;     /* or bad in this case */
		goto done2;
	}

	rc = stepd_terminate(fd, protocol_version);
	if (rc == -1)
		rc = ESLURMD_JOB_NOTRUNNING;

done2:
	close(fd);
done:
	slurm_send_rc_msg(msg, rc);
}
