smb2_is_status_pending(char *buf, struct TCP_Server_Info *server)
{
	struct smb2_hdr *shdr = (struct smb2_hdr *)buf;
	int scredits, in_flight;

	if (shdr->Status != STATUS_PENDING)
		return false;

	if (shdr->CreditRequest) {
		spin_lock(&server->req_lock);
		server->credits += le16_to_cpu(shdr->CreditRequest);
		scredits = server->credits;
		in_flight = server->in_flight;
		spin_unlock(&server->req_lock);
		wake_up(&server->request_q);

		trace_smb3_add_credits(server->CurrentMid,
				server->conn_id, server->hostname, scredits,
				le16_to_cpu(shdr->CreditRequest), in_flight);
		cifs_dbg(FYI, "%s: status pending add %u credits total=%d\n",
				__func__, le16_to_cpu(shdr->CreditRequest), scredits);
	}

	return true;
}