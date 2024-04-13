_rpc_acct_gather_energy(slurm_msg_t *msg)
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
		if (slurm_send_rc_msg(msg, rc) < 0)
			error("Error responding to energy request: %m");
	} else {
		slurm_msg_t resp_msg;
		acct_gather_node_resp_msg_t acct_msg;
		time_t now = time(NULL), last_poll = 0;
		int data_type = ENERGY_DATA_STRUCT;
		uint16_t sensor_cnt;
		acct_gather_energy_req_msg_t *req = msg->data;

		acct_gather_energy_g_get_data(ENERGY_DATA_LAST_POLL,
					      &last_poll);
		acct_gather_energy_g_get_data(ENERGY_DATA_SENSOR_CNT,
					      &sensor_cnt);

		/* If we polled later than delta seconds then force a
		   new poll.
		*/
		if ((now - last_poll) > req->delta)
			data_type = ENERGY_DATA_JOULES_TASK;

		memset(&acct_msg, 0, sizeof(acct_gather_node_resp_msg_t));
		acct_msg.sensor_cnt = sensor_cnt;
		acct_msg.energy = acct_gather_energy_alloc(acct_msg.sensor_cnt);

		acct_gather_energy_g_get_data(data_type, acct_msg.energy);

		slurm_msg_t_copy(&resp_msg, msg);
		resp_msg.msg_type = RESPONSE_ACCT_GATHER_ENERGY;
		resp_msg.data     = &acct_msg;

		slurm_send_node_msg(msg->conn_fd, &resp_msg);

		acct_gather_energy_destroy(acct_msg.energy);
	}
	return rc;
}
