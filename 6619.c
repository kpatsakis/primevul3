static Bool gf_fs_relocate_url(GF_FilterSession *session, const char *service_url, const char *parent_url, char *out_relocated_url, char *out_localized_url)
{
#ifdef GF_FS_ENABLE_LOCALES
	u32 i, count;

	count = gf_list_count(session->uri_relocators);
	for (i=0; i<count; i++) {
		Bool result;
		GF_URIRelocator *uri_relocator = gf_list_get(session->uri_relocators, i);
		result = uri_relocator->relocate_uri(uri_relocator, parent_url, service_url, out_relocated_url, out_localized_url);
		if (result) return 1;
	}
#endif
	return 0;
}