smb3_receive_transform(struct TCP_Server_Info *server,
		       struct mid_q_entry **mids, char **bufs, int *num_mids)
{
	char *buf = server->smallbuf;
	unsigned int pdu_length = server->pdu_size;
	struct smb2_transform_hdr *tr_hdr = (struct smb2_transform_hdr *)buf;
	unsigned int orig_len = le32_to_cpu(tr_hdr->OriginalMessageSize);

	if (pdu_length < sizeof(struct smb2_transform_hdr) +
						sizeof(struct smb2_hdr)) {
		cifs_server_dbg(VFS, "Transform message is too small (%u)\n",
			 pdu_length);
		cifs_reconnect(server, true);
		return -ECONNABORTED;
	}

	if (pdu_length < orig_len + sizeof(struct smb2_transform_hdr)) {
		cifs_server_dbg(VFS, "Transform message is broken\n");
		cifs_reconnect(server, true);
		return -ECONNABORTED;
	}

	/* TODO: add support for compounds containing READ. */
	if (pdu_length > CIFSMaxBufSize + MAX_HEADER_SIZE(server)) {
		return receive_encrypted_read(server, &mids[0], num_mids);
	}

	return receive_encrypted_standard(server, mids, bufs, num_mids);
}