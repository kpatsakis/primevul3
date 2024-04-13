Bool gf_filter_send_gf_event(GF_Filter *filter, GF_Event *evt)
{
	return gf_filter_forward_gf_event(filter, evt, GF_FALSE, GF_FALSE);
}