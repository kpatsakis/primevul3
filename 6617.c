Bool gf_filter_unclaim_opengl_provider(GF_Filter *filter, GF_VideoOutput * video_out)
{
	if (!filter || !video_out) return GF_FALSE;

	if (! (video_out->hw_caps & GF_VIDEO_HW_INTERNAL))
		return GF_FALSE;
	if (video_out != filter->session->gl_driver)
		return GF_FALSE;

	if (filter->session->gl_driver->hw_caps & GF_VIDEO_HW_ATTACHED) {
		filter->session->gl_driver->hw_caps &= ~GF_VIDEO_HW_ATTACHED;
		filter->session->gl_driver->on_event = fsess_on_event;
		filter->session->gl_driver->evt_cbk_hdl = filter->session;
		return GF_TRUE;
	}
	return GF_FALSE;
}