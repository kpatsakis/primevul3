UNCURL_EXPORT int32_t uncurl_new_tls_ctx(struct uncurl_tls_ctx **uc_tls_in)
{
	int32_t e;

	struct uncurl_tls_ctx *uc_tls = *uc_tls_in = calloc(1, sizeof(struct uncurl_tls_ctx));

	e = tlss_alloc(&uc_tls->tlss);
	if (e == UNCURL_OK) return e;

	uncurl_free_tls_ctx(uc_tls);
	*uc_tls_in = NULL;

	return e;
}
