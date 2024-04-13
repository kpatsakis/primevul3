smb2_adjust_credits(struct TCP_Server_Info *server,
		    struct cifs_credits *credits,
		    const unsigned int payload_size)
{
	int new_val = DIV_ROUND_UP(payload_size, SMB2_MAX_BUFFER_SIZE);
	int scredits, in_flight;

	if (!credits->value || credits->value == new_val)
		return 0;

	if (credits->value < new_val) {
		trace_smb3_too_many_credits(server->CurrentMid,
				server->conn_id, server->hostname, 0, credits->value - new_val, 0);
		cifs_server_dbg(VFS, "request has less credits (%d) than required (%d)",
				credits->value, new_val);

		return -ENOTSUPP;
	}

	spin_lock(&server->req_lock);

	if (server->reconnect_instance != credits->instance) {
		scredits = server->credits;
		in_flight = server->in_flight;
		spin_unlock(&server->req_lock);

		trace_smb3_reconnect_detected(server->CurrentMid,
			server->conn_id, server->hostname, scredits,
			credits->value - new_val, in_flight);
		cifs_server_dbg(VFS, "trying to return %d credits to old session\n",
			 credits->value - new_val);
		return -EAGAIN;
	}

	server->credits += credits->value - new_val;
	scredits = server->credits;
	in_flight = server->in_flight;
	spin_unlock(&server->req_lock);
	wake_up(&server->request_q);

	trace_smb3_add_credits(server->CurrentMid,
			server->conn_id, server->hostname, scredits,
			credits->value - new_val, in_flight);
	cifs_dbg(FYI, "%s: adjust added %u credits total=%d\n",
			__func__, credits->value - new_val, scredits);

	credits->value = new_val;

	return 0;
}