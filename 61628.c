_rpc_checkpoint_tasks(slurm_msg_t *msg)
{
	int               fd;
	int               rc = SLURM_SUCCESS;
	uid_t             req_uid = g_slurm_auth_get_uid(msg->auth_cred,
							 conf->auth_info);
	checkpoint_tasks_msg_t *req = (checkpoint_tasks_msg_t *) msg->data;
	uint16_t protocol_version;
	uid_t uid;

	fd = stepd_connect(conf->spooldir, conf->node_name,
			   req->job_id, req->job_step_id, &protocol_version);
	if (fd == -1) {
		debug("checkpoint for nonexistent %u.%u stepd_connect "
		      "failed: %m", req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done;
	}

	if ((int)(uid = stepd_get_uid(fd, protocol_version)) < 0) {
		debug("_rpc_checkpoint_tasks: couldn't read from the "
		      "step %u.%u: %m",
		      req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done2;
	}

	if ((req_uid != uid) && (!_slurm_authorized_user(req_uid))) {
		debug("checkpoint req from uid %ld for job %u.%u owned by "
		      "uid %ld", (long) req_uid, req->job_id, req->job_step_id,
		      (long) uid);
		rc = ESLURM_USER_ID_MISSING;     /* or bad in this case */
		goto done2;
	}

	rc = stepd_checkpoint(fd, protocol_version,
			      req->timestamp, req->image_dir);
	if (rc == -1)
		rc = ESLURMD_JOB_NOTRUNNING;

done2:
	close(fd);
done:
	slurm_send_rc_msg(msg, rc);
}
