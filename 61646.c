_rpc_step_complete_aggr(slurm_msg_t *msg)
{
	int rc;
	uid_t uid = g_slurm_auth_get_uid(msg->auth_cred, conf->auth_info);

	if (!_slurm_authorized_user(uid)) {
		error("Security violation: step_complete_aggr from uid %d",
		      uid);
		if (msg->conn_fd >= 0)
			slurm_send_rc_msg(msg, ESLURM_USER_ID_MISSING);
		return SLURM_ERROR;
	}

	if (conf->msg_aggr_window_msgs > 1) {
		slurm_msg_t *req = xmalloc_nz(sizeof(slurm_msg_t));
		_setup_step_complete_msg(req, msg->data);
		msg->data = NULL;

		msg_aggr_add_msg(req, 1, NULL);
	} else {
		slurm_msg_t req;
		_setup_step_complete_msg(&req, msg->data);

		while (slurm_send_recv_controller_rc_msg(&req, &rc) < 0) {
			error("Unable to send step complete, "
			      "trying again in a minute: %m");
		}
	}

	/* Finish communication with the stepd, we have to wait for
	 * the message back from the slurmctld or we will cause a race
	 * condition with srun.
	 */
	slurm_send_rc_msg(msg, SLURM_SUCCESS);

	return SLURM_SUCCESS;
}
