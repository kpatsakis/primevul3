_rpc_reconfig(slurm_msg_t *msg)
{
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);

	if (!_slurm_authorized_user(req_uid))
		error("Security violation, reconfig RPC from uid %d",
		      req_uid);
	else
		kill(conf->pid, SIGHUP);
	forward_wait(msg);
	/* Never return a message, slurmctld does not expect one */
}
