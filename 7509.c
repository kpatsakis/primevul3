smb3_negotiate_wsize(struct cifs_tcon *tcon, struct smb3_fs_context *ctx)
{
	struct TCP_Server_Info *server = tcon->ses->server;
	unsigned int wsize;

	/* start with specified wsize, or default */
	wsize = ctx->wsize ? ctx->wsize : SMB3_DEFAULT_IOSIZE;
	wsize = min_t(unsigned int, wsize, server->max_write);
#ifdef CONFIG_CIFS_SMB_DIRECT
	if (server->rdma) {
		if (server->sign)
			/*
			 * Account for SMB2 data transfer packet header and
			 * possible encryption header
			 */
			wsize = min_t(unsigned int,
				wsize,
				server->smbd_conn->max_fragmented_send_size -
					SMB2_READWRITE_PDU_HEADER_SIZE -
					sizeof(struct smb2_transform_hdr));
		else
			wsize = min_t(unsigned int,
				wsize, server->smbd_conn->max_readwrite_size);
	}
#endif
	if (!(server->capabilities & SMB2_GLOBAL_CAP_LARGE_MTU))
		wsize = min_t(unsigned int, wsize, SMB2_MAX_BUFFER_SIZE);

	return wsize;
}