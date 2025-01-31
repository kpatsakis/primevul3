gstd_close(void *the_tok)
{
	struct gstd_tok	*tok = the_tok;
	OM_uint32	 min;

	gss_delete_sec_context(&min, &tok->gstd_ctx, GSS_C_NO_BUFFER);
	if (tok->gstd_inbuf.length > 0)
		gss_release_buffer(&min, &tok->gstd_inbuf);
	close(tok->gstd_fd);
	return 0;
}
