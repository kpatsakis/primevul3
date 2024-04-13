_rpc_health_check(slurm_msg_t *msg)
{
	int        rc = SLURM_SUCCESS;
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);

	if (!_slurm_authorized_user(req_uid)) {
		error("Security violation, health check RPC from uid %d",
		      req_uid);
		rc = ESLURM_USER_ID_MISSING;	/* or bad in this case */
	}

	/* Return result. If the reply can't be sent this indicates that
	 * 1. The network is broken OR
	 * 2. slurmctld has died    OR
	 * 3. slurmd was paged out due to full memory
	 * If the reply request fails, we send an registration message to
	 * slurmctld in hopes of avoiding having the node set DOWN due to
	 * slurmd paging and not being able to respond in a timely fashion. */
	if (slurm_send_rc_msg(msg, rc) < 0) {
		error("Error responding to health check: %m");
		send_registration_msg(SLURM_SUCCESS, false);
	}

	if (rc == SLURM_SUCCESS)
		rc = run_script_health_check();

	/* Take this opportunity to enforce any job memory limits */
	_enforce_job_mem_limit();
	/* Clear up any stalled file transfers as well */
	_file_bcast_cleanup();
	return rc;
}
