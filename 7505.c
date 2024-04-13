smb3_negotiate_rsize(struct cifs_tcon *tcon, struct smb3_fs_context *ctx)
{
	struct TCP_Server_Info *server = tcon->ses->server;
	unsigned int rsize;

	/* start with specified rsize, or default */
	rsize = ctx->rsize ? ctx->rsize : SMB3_DEFAULT_IOSIZE;
	rsize = min_t(unsigned int, rsize, server->max_read);
#ifdef CONFIG_CIFS_SMB_DIRECT
	if (server->rdma) {
		if (server->sign)
			/*
			 * Account for SMB2 data transfer packet header and
			 * possible encryption header
			 */
			rsize = min_t(unsigned int,
				rsize,
				server->smbd_conn->max_fragmented_recv_size -
					SMB2_READWRITE_PDU_HEADER_SIZE -
					sizeof(struct smb2_transform_hdr));
		else
			rsize = min_t(unsigned int,
				rsize, server->smbd_conn->max_readwrite_size);
	}
#endif

	if (!(server->capabilities & SMB2_GLOBAL_CAP_LARGE_MTU))
		rsize = min_t(unsigned int, rsize, SMB2_MAX_BUFFER_SIZE);

	return rsize;
}