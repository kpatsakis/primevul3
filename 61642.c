_rpc_shutdown(slurm_msg_t *msg)
{
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);

	forward_wait(msg);
	if (!_slurm_authorized_user(req_uid))
		error("Security violation, shutdown RPC from uid %d",
		      req_uid);
	else {
		if (kill(conf->pid, SIGTERM) != 0)
			error("kill(%u,SIGTERM): %m", conf->pid);
	}

	/* Never return a message, slurmctld does not expect one */
}
