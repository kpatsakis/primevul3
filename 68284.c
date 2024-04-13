UNCURL_EXPORT int32_t uncurl_set_cacert_file(struct uncurl_tls_ctx *uc_tls, char *cacert_file)
{
	return tlss_load_cacert_file(uc_tls->tlss, cacert_file);
}
