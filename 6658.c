void gf_filter_set_session_caps(GF_Filter *filter, GF_FilterSessionCaps *caps)
{
	if (caps && filter) {
		filter->session->caps = (*caps);
		//TODO fire event
	}
}