_rpc_acct_gather_update(slurm_msg_t *msg)
{
	int        rc = SLURM_SUCCESS;
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);
	static bool first_msg = true;

	if (!_slurm_authorized_user(req_uid)) {
		error("Security violation, acct_gather_update RPC from uid %d",
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
			error("Error responding to account gather: %m");
			send_registration_msg(SLURM_SUCCESS, false);
		}
	} else {
		slurm_msg_t resp_msg;
		acct_gather_node_resp_msg_t acct_msg;

		/* Update node energy usage data */
		acct_gather_energy_g_update_node_energy();

		memset(&acct_msg, 0, sizeof(acct_gather_node_resp_msg_t));
		acct_msg.node_name = conf->node_name;
		acct_msg.sensor_cnt = 1;
		acct_msg.energy = acct_gather_energy_alloc(acct_msg.sensor_cnt);
		acct_gather_energy_g_get_data(
			ENERGY_DATA_NODE_ENERGY, acct_msg.energy);

		slurm_msg_t_copy(&resp_msg, msg);
		resp_msg.msg_type = RESPONSE_ACCT_GATHER_UPDATE;
		resp_msg.data     = &acct_msg;

		slurm_send_node_msg(msg->conn_fd, &resp_msg);

		acct_gather_energy_destroy(acct_msg.energy);
	}
	return rc;
}
