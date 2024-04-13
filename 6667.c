struct _gf_ft_mgr *gf_filter_get_font_manager(GF_Filter *filter)
{
	if (!filter) return NULL;
	return gf_fs_get_font_manager(filter->session);
}