_rpc_ping(slurm_msg_t *msg)
{
	int        rc = SLURM_SUCCESS;
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);
	static bool first_msg = true;

	if (!_slurm_authorized_user(req_uid)) {
		error("Security violation, ping RPC from uid %d",
		      req_uid);
		if (first_msg) {
			error("Do you have SlurmUser configured as uid %d?",
			      req_uid);
		}
		rc = ESLURM_USER_ID_MISSING;	/* or bad in this case */
	}
	first_msg = false;

	if (rc != SLURM_SUCCESS) {
		/* Return result. If the reply can't be sent this indicates
		 * 1. The network is broken OR
		 * 2. slurmctld has died    OR
		 * 3. slurmd was paged out due to full memory
		 * If the reply request fails, we send an registration message
		 * to slurmctld in hopes of avoiding having the node set DOWN
		 * due to slurmd paging and not being able to respond in a
		 * timely fashion. */
		if (slurm_send_rc_msg(msg, rc) < 0) {
			error("Error responding to ping: %m");
			send_registration_msg(SLURM_SUCCESS, false);
		}
	} else {
		slurm_msg_t resp_msg;
		ping_slurmd_resp_msg_t ping_resp;
		get_cpu_load(&ping_resp.cpu_load);
		get_free_mem(&ping_resp.free_mem);
		slurm_msg_t_copy(&resp_msg, msg);
		resp_msg.msg_type = RESPONSE_PING_SLURMD;
		resp_msg.data     = &ping_resp;

		slurm_send_node_msg(msg->conn_fd, &resp_msg);
	}

	/* Take this opportunity to enforce any job memory limits */
	_enforce_job_mem_limit();
	/* Clear up any stalled file transfers as well */
	_file_bcast_cleanup();
	return rc;
}
