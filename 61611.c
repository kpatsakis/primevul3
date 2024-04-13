_launch_job_fail(uint32_t job_id, uint32_t slurm_rc)
{
	complete_batch_script_msg_t comp_msg;
	struct requeue_msg req_msg;
	slurm_msg_t resp_msg;
	int rc = 0, rpc_rc;
	static time_t config_update = 0;
	static bool requeue_no_hold = false;

	if (config_update != conf->last_update) {
		char *sched_params = slurm_get_sched_params();
		requeue_no_hold = (sched_params && strstr(
					   sched_params,
					   "nohold_on_prolog_fail"));
		xfree(sched_params);
		config_update = conf->last_update;
	}

	slurm_msg_t_init(&resp_msg);

	if (slurm_rc == ESLURMD_CREDENTIAL_REVOKED) {
		comp_msg.job_id = job_id;
		comp_msg.job_rc = INFINITE;
		comp_msg.slurm_rc = slurm_rc;
		comp_msg.node_name = conf->node_name;
		comp_msg.jobacct = NULL; /* unused */
		resp_msg.msg_type = REQUEST_COMPLETE_BATCH_SCRIPT;
		resp_msg.data = &comp_msg;
	} else {
		req_msg.job_id = job_id;
		req_msg.job_id_str = NULL;
		if (requeue_no_hold) {
			req_msg.state = JOB_PENDING;
		} else {
			req_msg.state = (JOB_REQUEUE_HOLD|JOB_LAUNCH_FAILED);
		}
		resp_msg.msg_type = REQUEST_JOB_REQUEUE;
		resp_msg.data = &req_msg;
	}

	rpc_rc = slurm_send_recv_controller_rc_msg(&resp_msg, &rc);
	if ((resp_msg.msg_type == REQUEST_JOB_REQUEUE) &&
	    ((rc == ESLURM_DISABLED) || (rc == ESLURM_BATCH_ONLY))) {
		info("Could not launch job %u and not able to requeue it, "
		     "cancelling job", job_id);

		if ((slurm_rc == ESLURMD_PROLOG_FAILED) &&
		    (rc == ESLURM_BATCH_ONLY)) {
			char *buf = NULL;
			xstrfmtcat(buf, "Prolog failure on node %s",
				   conf->node_name);
			slurm_notify_job(job_id, buf);
			xfree(buf);
		}

		comp_msg.job_id = job_id;
		comp_msg.job_rc = INFINITE;
		comp_msg.slurm_rc = slurm_rc;
		comp_msg.node_name = conf->node_name;
		comp_msg.jobacct = NULL; /* unused */
		resp_msg.msg_type = REQUEST_COMPLETE_BATCH_SCRIPT;
		resp_msg.data = &comp_msg;
		rpc_rc = slurm_send_recv_controller_rc_msg(&resp_msg, &rc);
	}

	return rpc_rc;
}
