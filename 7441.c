receive_encrypted_standard(struct TCP_Server_Info *server,
			   struct mid_q_entry **mids, char **bufs,
			   int *num_mids)
{
	int ret, length;
	char *buf = server->smallbuf;
	struct smb2_hdr *shdr;
	unsigned int pdu_length = server->pdu_size;
	unsigned int buf_size;
	struct mid_q_entry *mid_entry;
	int next_is_large;
	char *next_buffer = NULL;

	*num_mids = 0;

	/* switch to large buffer if too big for a small one */
	if (pdu_length > MAX_CIFS_SMALL_BUFFER_SIZE) {
		server->large_buf = true;
		memcpy(server->bigbuf, buf, server->total_read);
		buf = server->bigbuf;
	}

	/* now read the rest */
	length = cifs_read_from_socket(server, buf + HEADER_SIZE(server) - 1,
				pdu_length - HEADER_SIZE(server) + 1);
	if (length < 0)
		return length;
	server->total_read += length;

	buf_size = pdu_length - sizeof(struct smb2_transform_hdr);
	length = decrypt_raw_data(server, buf, buf_size, NULL, 0, 0, false);
	if (length)
		return length;

	next_is_large = server->large_buf;
one_more:
	shdr = (struct smb2_hdr *)buf;
	if (shdr->NextCommand) {
		if (next_is_large)
			next_buffer = (char *)cifs_buf_get();
		else
			next_buffer = (char *)cifs_small_buf_get();
		memcpy(next_buffer,
		       buf + le32_to_cpu(shdr->NextCommand),
		       pdu_length - le32_to_cpu(shdr->NextCommand));
	}

	mid_entry = smb2_find_mid(server, buf);
	if (mid_entry == NULL)
		cifs_dbg(FYI, "mid not found\n");
	else {
		cifs_dbg(FYI, "mid found\n");
		mid_entry->decrypted = true;
		mid_entry->resp_buf_size = server->pdu_size;
	}

	if (*num_mids >= MAX_COMPOUND) {
		cifs_server_dbg(VFS, "too many PDUs in compound\n");
		return -1;
	}
	bufs[*num_mids] = buf;
	mids[(*num_mids)++] = mid_entry;

	if (mid_entry && mid_entry->handle)
		ret = mid_entry->handle(server, mid_entry);
	else
		ret = cifs_handle_standard(server, mid_entry);

	if (ret == 0 && shdr->NextCommand) {
		pdu_length -= le32_to_cpu(shdr->NextCommand);
		server->large_buf = next_is_large;
		if (next_is_large)
			server->bigbuf = buf = next_buffer;
		else
			server->smallbuf = buf = next_buffer;
		goto one_more;
	} else if (ret != 0) {
		/*
		 * ret != 0 here means that we didn't get to handle_mid() thus
		 * server->smallbuf and server->bigbuf are still valid. We need
		 * to free next_buffer because it is not going to be used
		 * anywhere.
		 */
		if (next_is_large)
			free_rsp_buf(CIFS_LARGE_BUFFER, next_buffer);
		else
			free_rsp_buf(CIFS_SMALL_BUFFER, next_buffer);
	}

	return ret;
}