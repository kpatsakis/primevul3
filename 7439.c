smb2_add_credits(struct TCP_Server_Info *server,
		 const struct cifs_credits *credits, const int optype)
{
	int *val, rc = -1;
	int scredits, in_flight;
	unsigned int add = credits->value;
	unsigned int instance = credits->instance;
	bool reconnect_detected = false;
	bool reconnect_with_invalid_credits = false;

	spin_lock(&server->req_lock);
	val = server->ops->get_credits_field(server, optype);

	/* eg found case where write overlapping reconnect messed up credits */
	if (((optype & CIFS_OP_MASK) == CIFS_NEG_OP) && (*val != 0))
		reconnect_with_invalid_credits = true;

	if ((instance == 0) || (instance == server->reconnect_instance))
		*val += add;
	else
		reconnect_detected = true;

	if (*val > 65000) {
		*val = 65000; /* Don't get near 64K credits, avoid srv bugs */
		pr_warn_once("server overflowed SMB3 credits\n");
	}
	server->in_flight--;
	if (server->in_flight == 0 &&
	   ((optype & CIFS_OP_MASK) != CIFS_NEG_OP) &&
	   ((optype & CIFS_OP_MASK) != CIFS_SESS_OP))
		rc = change_conf(server);
	/*
	 * Sometimes server returns 0 credits on oplock break ack - we need to
	 * rebalance credits in this case.
	 */
	else if (server->in_flight > 0 && server->oplock_credits == 0 &&
		 server->oplocks) {
		if (server->credits > 1) {
			server->credits--;
			server->oplock_credits++;
		}
	}
	scredits = *val;
	in_flight = server->in_flight;
	spin_unlock(&server->req_lock);
	wake_up(&server->request_q);

	if (reconnect_detected) {
		trace_smb3_reconnect_detected(server->CurrentMid,
			server->conn_id, server->hostname, scredits, add, in_flight);

		cifs_dbg(FYI, "trying to put %d credits from the old server instance %d\n",
			 add, instance);
	}

	if (reconnect_with_invalid_credits) {
		trace_smb3_reconnect_with_invalid_credits(server->CurrentMid,
			server->conn_id, server->hostname, scredits, add, in_flight);
		cifs_dbg(FYI, "Negotiate operation when server credits is non-zero. Optype: %d, server credits: %d, credits added: %d\n",
			 optype, scredits, add);
	}

	spin_lock(&cifs_tcp_ses_lock);
	if (server->tcpStatus == CifsNeedReconnect
	    || server->tcpStatus == CifsExiting) {
		spin_unlock(&cifs_tcp_ses_lock);
		return;
	}
	spin_unlock(&cifs_tcp_ses_lock);

	switch (rc) {
	case -1:
		/* change_conf hasn't been executed */
		break;
	case 0:
		cifs_server_dbg(VFS, "Possible client or server bug - zero credits\n");
		break;
	case 1:
		cifs_server_dbg(VFS, "disabling echoes and oplocks\n");
		break;
	case 2:
		cifs_dbg(FYI, "disabling oplocks\n");
		break;
	default:
		/* change_conf rebalanced credits for different types */
		break;
	}

	trace_smb3_add_credits(server->CurrentMid,
			server->conn_id, server->hostname, scredits, add, in_flight);
	cifs_dbg(FYI, "%s: added %u credits total=%d\n", __func__, add, scredits);
}