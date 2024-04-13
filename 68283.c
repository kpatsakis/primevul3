UNCURL_EXPORT int32_t uncurl_set_cacert(struct uncurl_tls_ctx *uc_tls, char *cacert, size_t size)
{
	return tlss_load_cacert(uc_tls->tlss, cacert, size);
}
