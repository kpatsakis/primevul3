smb2_negotiate_rsize(struct cifs_tcon *tcon, struct smb3_fs_context *ctx)
{
	struct TCP_Server_Info *server = tcon->ses->server;
	unsigned int rsize;

	/* start with specified rsize, or default */
	rsize = ctx->rsize ? ctx->rsize : CIFS_DEFAULT_IOSIZE;
	rsize = min_t(unsigned int, rsize, server->max_read);

	if (!(server->capabilities & SMB2_GLOBAL_CAP_LARGE_MTU))
		rsize = min_t(unsigned int, rsize, SMB2_MAX_BUFFER_SIZE);

	return rsize;
}