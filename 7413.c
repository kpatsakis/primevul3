smb2_set_credits(struct TCP_Server_Info *server, const int val)
{
	int scredits, in_flight;

	spin_lock(&server->req_lock);
	server->credits = val;
	if (val == 1)
		server->reconnect_instance++;
	scredits = server->credits;
	in_flight = server->in_flight;
	spin_unlock(&server->req_lock);

	trace_smb3_set_credits(server->CurrentMid,
			server->conn_id, server->hostname, scredits, val, in_flight);
	cifs_dbg(FYI, "%s: set %u credits\n", __func__, val);

	/* don't log while holding the lock */
	if (val == 1)
		cifs_dbg(FYI, "set credits to 1 due to smb2 reconnect\n");
}