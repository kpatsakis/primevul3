gstd_release_context(void *ctx) {
	OM_uint32	min;

	gss_delete_sec_context(&min, (gss_ctx_id_t *)ctx, GSS_C_NO_BUFFER);
}
