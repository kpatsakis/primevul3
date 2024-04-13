GF_Err gf_fs_set_gl(GF_FilterSession *session)
{
	GF_Event evt;
	if (!session->gl_driver) return GF_BAD_PARAM;
	memset(&evt, 0, sizeof(GF_Event));
	evt.type = GF_EVENT_SET_GL;
	return session->gl_driver->ProcessEvent(session->gl_driver, &evt);
}