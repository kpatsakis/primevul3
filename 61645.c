_rpc_step_complete(slurm_msg_t *msg)
{
	step_complete_msg_t *req = (step_complete_msg_t *)msg->data;
	int               rc = SLURM_SUCCESS;
	int               fd;
	uid_t             req_uid;
	uint16_t protocol_version;

	debug3("Entering _rpc_step_complete");
	fd = stepd_connect(conf->spooldir, conf->node_name,
			   req->job_id, req->job_step_id, &protocol_version);
	if (fd == -1) {
		error("stepd_connect to %u.%u failed: %m",
		      req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done;
	}

	/* step completion messages are only allowed from other slurmstepd,
	   so only root or SlurmUser is allowed here */
	req_uid = g_slurm_auth_get_uid(msg->auth_cred, conf->auth_info);
	if (!_slurm_authorized_user(req_uid)) {
		debug("step completion from uid %ld for job %u.%u",
		      (long) req_uid, req->job_id, req->job_step_id);
		rc = ESLURM_USER_ID_MISSING;     /* or bad in this case */
		goto done2;
	}

	rc = stepd_completion(fd, protocol_version, req);
	if (rc == -1)
		rc = ESLURMD_JOB_NOTRUNNING;

done2:
	close(fd);
done:
	slurm_send_rc_msg(msg, rc);

	return rc;
}
