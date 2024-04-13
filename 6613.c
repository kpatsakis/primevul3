Bool gf_fs_ui_event(GF_FilterSession *session, GF_Event *uievt)
{
	Bool ret;
	gf_mx_p(session->ui_mx);
	ret = session->ui_event_proc(session->ui_opaque, uievt);
	gf_mx_v(session->ui_mx);
	return ret;
}