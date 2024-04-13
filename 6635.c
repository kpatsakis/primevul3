Bool gf_filter_forward_gf_event(GF_Filter *filter, GF_Event *evt, Bool consumed, Bool skip_user)
{
	if (!filter) return GF_FALSE;
	return gf_fs_forward_gf_event(filter->session, evt, consumed, skip_user);

}