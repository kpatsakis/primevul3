GF_VideoOutput *gf_filter_claim_opengl_provider(GF_Filter *filter)
{
	if (!filter || !filter->session || !filter->session->gl_driver) return NULL;

	if (! (filter->session->gl_driver->hw_caps & GF_VIDEO_HW_INTERNAL))
		return NULL;
	if (filter->session->gl_driver->hw_caps & GF_VIDEO_HW_ATTACHED)
		return NULL;

	filter->session->gl_driver->hw_caps |= GF_VIDEO_HW_ATTACHED;
	return filter->session->gl_driver;
}