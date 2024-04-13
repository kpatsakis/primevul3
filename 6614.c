Bool gf_filter_relocate_url(GF_Filter *filter, const char *service_url, const char *parent_url, char *out_relocated_url, char *out_localized_url)
{
	if (!filter) return 0;
	return gf_fs_relocate_url(filter->session, service_url, parent_url, out_relocated_url, out_localized_url);
}