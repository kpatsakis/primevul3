Bool gf_fs_is_supported_source(GF_FilterSession *session, const char *url, const char *parent_url)
{
	GF_Err e;
	Bool is_supported = GF_FALSE;
	gf_fs_load_source_dest_internal(session, url, NULL, parent_url, &e, NULL, NULL, GF_TRUE, GF_TRUE, &is_supported);
	return is_supported;
}