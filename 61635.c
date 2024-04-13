_rpc_network_callerid(slurm_msg_t *msg)
{
	network_callerid_msg_t *req = (network_callerid_msg_t *)msg->data;
	slurm_msg_t resp_msg;
	network_callerid_resp_t *resp = NULL;

	uid_t req_uid = -1;
	uid_t job_uid = -1;
	uint32_t job_id = (uint32_t)NO_VAL;
	callerid_conn_t conn;
	int rc = ESLURM_INVALID_JOB_ID;
	char ip_src_str[INET6_ADDRSTRLEN];
	char ip_dst_str[INET6_ADDRSTRLEN];

	debug3("Entering _rpc_network_callerid");

	resp = xmalloc(sizeof(network_callerid_resp_t));
	slurm_msg_t_copy(&resp_msg, msg);

	/* Ideally this would be in an if block only when debug3 is enabled */
	inet_ntop(req->af, req->ip_src, ip_src_str, INET6_ADDRSTRLEN);
	inet_ntop(req->af, req->ip_dst, ip_dst_str, INET6_ADDRSTRLEN);
	debug3("network_callerid checking %s:%u => %s:%u",
		ip_src_str, req->port_src, ip_dst_str, req->port_dst);

	/* My remote is the other's source */
	memcpy((void*)&conn.ip_dst, (void*)&req->ip_src, 16);
	memcpy((void*)&conn.ip_src, (void*)&req->ip_dst, 16);
	conn.port_src = req->port_dst;
	conn.port_dst = req->port_src;
	conn.af = req->af;

	/* Find the job id */
	rc = _callerid_find_job(conn, &job_id);
	if (rc == SLURM_SUCCESS) {
		/* We found the job */
		req_uid = g_slurm_auth_get_uid(msg->auth_cred, conf->auth_info);
		if (!_slurm_authorized_user(req_uid)) {
			/* Requestor is not root or SlurmUser */
			job_uid = _get_job_uid(job_id);
			if (job_uid != req_uid) {
				/* RPC call sent by non-root user who does not
				 * own this job. Do not send them the job ID. */
				error("Security violation, REQUEST_NETWORK_CALLERID from uid=%d",
				      req_uid);
				job_id = NO_VAL;
				rc = ESLURM_INVALID_JOB_ID;
			}
		}
	}

	resp->job_id = job_id;
	resp->node_name = xstrdup(conf->node_name);

	resp_msg.msg_type = RESPONSE_NETWORK_CALLERID;
	resp_msg.data     = resp;

	slurm_send_node_msg(msg->conn_fd, &resp_msg);
	slurm_free_network_callerid_resp(resp);
	return rc;
}
