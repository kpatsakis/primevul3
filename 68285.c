UNCURL_EXPORT int32_t uncurl_set_cert_and_key_file(struct uncurl_tls_ctx *uc_tls, char *cert_file, char *key_file)
{
	return tlss_load_cert_and_key_file(uc_tls->tlss, cert_file, key_file);
}
