smb2_negotiate_wsize(struct cifs_tcon *tcon, struct smb3_fs_context *ctx)
{
	struct TCP_Server_Info *server = tcon->ses->server;
	unsigned int wsize;

	/* start with specified wsize, or default */
	wsize = ctx->wsize ? ctx->wsize : CIFS_DEFAULT_IOSIZE;
	wsize = min_t(unsigned int, wsize, server->max_write);
	if (!(server->capabilities & SMB2_GLOBAL_CAP_LARGE_MTU))
		wsize = min_t(unsigned int, wsize, SMB2_MAX_BUFFER_SIZE);

	return wsize;
}