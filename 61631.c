_rpc_forward_data(slurm_msg_t *msg)
{
	forward_data_msg_t *req = (forward_data_msg_t *)msg->data;
	uint32_t req_uid;
	struct sockaddr_un sa;
	int fd = -1, rc = 0;

	debug3("Entering _rpc_forward_data, address: %s, len: %u",
	       req->address, req->len);

	/* sanity check */
	if (strlen(req->address) > sizeof(sa.sun_path) - 1) {
		slurm_seterrno(EINVAL);
		rc = errno;
		goto done;
	}

	/* connect to specified address */
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		rc = errno;
		error("failed creating UNIX domain socket: %m");
		goto done;
	}
	memset(&sa, 0, sizeof(sa));
	sa.sun_family = AF_UNIX;
	strcpy(sa.sun_path, req->address);
	while (((rc = connect(fd, (struct sockaddr *)&sa, SUN_LEN(&sa))) < 0) &&
	       (errno == EINTR));
	if (rc < 0) {
		rc = errno;
		debug2("failed connecting to specified socket '%s': %m",
		       req->address);
		goto done;
	}

	req_uid = (uint32_t)g_slurm_auth_get_uid(msg->auth_cred,
						 conf->auth_info);
	/*
	 * although always in localhost, we still convert it to network
	 * byte order, to make it consistent with pack/unpack.
	 */
	req_uid = htonl(req_uid);
	safe_write(fd, &req_uid, sizeof(uint32_t));
	req_uid = htonl(req->len);
	safe_write(fd, &req_uid, sizeof(uint32_t));
	safe_write(fd, req->data, req->len);

rwfail:
done:
	if (fd >= 0){
		close(fd);
	}
	slurm_send_rc_msg(msg, rc);
}
