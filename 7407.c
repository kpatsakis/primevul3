smb3_handle_read_data(struct TCP_Server_Info *server, struct mid_q_entry *mid)
{
	char *buf = server->large_buf ? server->bigbuf : server->smallbuf;

	return handle_read_data(server, mid, buf, server->pdu_size,
				NULL, 0, 0, false);
}