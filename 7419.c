smb2_wait_mtu_credits(struct TCP_Server_Info *server, unsigned int size,
		      unsigned int *num, struct cifs_credits *credits)
{
	int rc = 0;
	unsigned int scredits, in_flight;

	spin_lock(&server->req_lock);
	while (1) {
		if (server->credits <= 0) {
			spin_unlock(&server->req_lock);
			cifs_num_waiters_inc(server);
			rc = wait_event_killable(server->request_q,
				has_credits(server, &server->credits, 1));
			cifs_num_waiters_dec(server);
			if (rc)
				return rc;
			spin_lock(&server->req_lock);
		} else {
			spin_unlock(&server->req_lock);
			spin_lock(&cifs_tcp_ses_lock);
			if (server->tcpStatus == CifsExiting) {
				spin_unlock(&cifs_tcp_ses_lock);
				return -ENOENT;
			}
			spin_unlock(&cifs_tcp_ses_lock);

			spin_lock(&server->req_lock);
			scredits = server->credits;
			/* can deadlock with reopen */
			if (scredits <= 8) {
				*num = SMB2_MAX_BUFFER_SIZE;
				credits->value = 0;
				credits->instance = 0;
				break;
			}

			/* leave some credits for reopen and other ops */
			scredits -= 8;
			*num = min_t(unsigned int, size,
				     scredits * SMB2_MAX_BUFFER_SIZE);

			credits->value =
				DIV_ROUND_UP(*num, SMB2_MAX_BUFFER_SIZE);
			credits->instance = server->reconnect_instance;
			server->credits -= credits->value;
			server->in_flight++;
			if (server->in_flight > server->max_in_flight)
				server->max_in_flight = server->in_flight;
			break;
		}
	}
	scredits = server->credits;
	in_flight = server->in_flight;
	spin_unlock(&server->req_lock);

	trace_smb3_add_credits(server->CurrentMid,
			server->conn_id, server->hostname, scredits, -(credits->value), in_flight);
	cifs_dbg(FYI, "%s: removed %u credits total=%d\n",
			__func__, credits->value, scredits);

	return rc;
}