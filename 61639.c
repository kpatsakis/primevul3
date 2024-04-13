_rpc_reattach_tasks(slurm_msg_t *msg)
{
	reattach_tasks_request_msg_t  *req = msg->data;
	reattach_tasks_response_msg_t *resp =
		xmalloc(sizeof(reattach_tasks_response_msg_t));
	slurm_msg_t                    resp_msg;
	int          rc   = SLURM_SUCCESS;
	uint16_t     port = 0;
	char         host[MAXHOSTNAMELEN];
	slurm_addr_t   ioaddr;
	void        *job_cred_sig;
	uint32_t     len;
	int               fd;
	uid_t             req_uid;
	slurm_addr_t *cli = &msg->orig_addr;
	uint32_t nodeid = (uint32_t)NO_VAL;
	uid_t uid = -1;
	uint16_t protocol_version;

	slurm_msg_t_copy(&resp_msg, msg);
	fd = stepd_connect(conf->spooldir, conf->node_name,
			   req->job_id, req->job_step_id, &protocol_version);
	if (fd == -1) {
		debug("reattach for nonexistent job %u.%u stepd_connect"
		      " failed: %m", req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done;
	}

	if ((int)(uid = stepd_get_uid(fd, protocol_version)) < 0) {
		debug("_rpc_reattach_tasks couldn't read from the "
		      "step %u.%u: %m",
		      req->job_id, req->job_step_id);
		rc = ESLURM_INVALID_JOB_ID;
		goto done2;
	}

	nodeid = stepd_get_nodeid(fd, protocol_version);

	debug2("_rpc_reattach_tasks: nodeid %d in the job step", nodeid);

	req_uid = g_slurm_auth_get_uid(msg->auth_cred, conf->auth_info);
	if ((req_uid != uid) && (!_slurm_authorized_user(req_uid))) {
		error("uid %ld attempt to attach to job %u.%u owned by %ld",
		      (long) req_uid, req->job_id, req->job_step_id,
		      (long) uid);
		rc = EPERM;
		goto done2;
	}

	memset(resp, 0, sizeof(reattach_tasks_response_msg_t));
	slurm_get_ip_str(cli, &port, host, sizeof(host));

	/*
	 * Set response address by resp_port and client address
	 */
	memcpy(&resp_msg.address, cli, sizeof(slurm_addr_t));
	if (req->num_resp_port > 0) {
		port = req->resp_port[nodeid % req->num_resp_port];
		slurm_set_addr(&resp_msg.address, port, NULL);
	}

	/*
	 * Set IO address by io_port and client address
	 */
	memcpy(&ioaddr, cli, sizeof(slurm_addr_t));

	if (req->num_io_port > 0) {
		port = req->io_port[nodeid % req->num_io_port];
		slurm_set_addr(&ioaddr, port, NULL);
	}

	/*
	 * Get the signature of the job credential.  slurmstepd will need
	 * this to prove its identity when it connects back to srun.
	 */
	slurm_cred_get_signature(req->cred, (char **)(&job_cred_sig), &len);
	if (len != SLURM_IO_KEY_SIZE) {
		error("Incorrect slurm cred signature length");
		goto done2;
	}

	resp->gtids = NULL;
	resp->local_pids = NULL;

	 /* NOTE: We need to use the protocol_version from
	  * sattach here since responses will be sent back to it. */
	if (msg->protocol_version < protocol_version)
		protocol_version = msg->protocol_version;

	/* Following call fills in gtids and local_pids when successful. */
	rc = stepd_attach(fd, protocol_version, &ioaddr,
			  &resp_msg.address, job_cred_sig, resp);
	if (rc != SLURM_SUCCESS) {
		debug2("stepd_attach call failed");
		goto done2;
	}

done2:
	close(fd);
done:
	debug2("update step addrs rc = %d", rc);
	resp_msg.data         = resp;
	resp_msg.msg_type     = RESPONSE_REATTACH_TASKS;
	resp->node_name       = xstrdup(conf->node_name);
	resp->return_code     = rc;
	debug2("node %s sending rc = %d", conf->node_name, rc);

	slurm_send_node_msg(msg->conn_fd, &resp_msg);
	slurm_free_reattach_tasks_response_msg(resp);
}
