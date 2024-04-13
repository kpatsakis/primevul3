_rpc_daemon_status(slurm_msg_t *msg)
{
	slurm_msg_t      resp_msg;
	slurmd_status_t *resp = NULL;

	resp = xmalloc(sizeof(slurmd_status_t));
	resp->actual_cpus        = conf->actual_cpus;
	resp->actual_boards      = conf->actual_boards;
	resp->actual_sockets     = conf->actual_sockets;
	resp->actual_cores       = conf->actual_cores;
	resp->actual_threads     = conf->actual_threads;
	resp->actual_real_mem    = conf->real_memory_size;
	resp->actual_tmp_disk    = conf->tmp_disk_space;
	resp->booted             = startup;
	resp->hostname           = xstrdup(conf->node_name);
	resp->step_list          = _get_step_list();
	resp->last_slurmctld_msg = last_slurmctld_msg;
	resp->pid                = conf->pid;
	resp->slurmd_debug       = conf->debug_level;
	resp->slurmd_logfile     = xstrdup(conf->logfile);
	resp->version            = xstrdup(SLURM_VERSION_STRING);

	slurm_msg_t_copy(&resp_msg, msg);
	resp_msg.msg_type = RESPONSE_SLURMD_STATUS;
	resp_msg.data     = resp;
	slurm_send_node_msg(msg->conn_fd, &resp_msg);
	slurm_free_slurmd_status(resp);
	return SLURM_SUCCESS;
}
