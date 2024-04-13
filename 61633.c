_rpc_job_notify(slurm_msg_t *msg)
{
	job_notify_msg_t *req = msg->data;
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);
	uid_t job_uid;
	List steps;
	ListIterator i;
	step_loc_t *stepd = NULL;
	int step_cnt  = 0;
	int fd;

	debug("_rpc_job_notify, uid = %d, jobid = %u", req_uid, req->job_id);
	job_uid = _get_job_uid(req->job_id);
	if ((int)job_uid < 0)
		goto no_job;

	/*
	 * check that requesting user ID is the SLURM UID or root
	 */
	if ((req_uid != job_uid) && (!_slurm_authorized_user(req_uid))) {
		error("Security violation: job_notify(%u) from uid %d",
		      req->job_id, req_uid);
		return;
	}

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((stepd = list_next(i))) {
		if ((stepd->jobid  != req->job_id) ||
		    (stepd->stepid != SLURM_BATCH_SCRIPT)) {
			continue;
		}

		step_cnt++;

		fd = stepd_connect(stepd->directory, stepd->nodename,
				   stepd->jobid, stepd->stepid,
				   &stepd->protocol_version);
		if (fd == -1) {
			debug3("Unable to connect to step %u.%u",
			       stepd->jobid, stepd->stepid);
			continue;
		}

		info("send notification to job %u.%u",
		     stepd->jobid, stepd->stepid);
		if (stepd_notify_job(fd, stepd->protocol_version,
				     req->message) < 0)
			debug("notify jobid=%u failed: %m", stepd->jobid);
		close(fd);
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);

no_job:
	if (step_cnt == 0) {
		debug2("Can't find jobid %u to send notification message",
		       req->job_id);
	}
}
