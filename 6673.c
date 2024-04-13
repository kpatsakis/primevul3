GF_Err gf_fs_check_gl_provider(GF_FilterSession *session)
{
	GF_Event evt;
	GF_Err e;
	const char *sOpt;
	void *os_disp_handler;

	if (!session->nb_gl_filters) return GF_OK;
	if (gf_list_count(session->gl_providers)) return GF_OK;

	if (session->gl_driver) return GF_OK;


	session->gl_driver = (GF_VideoOutput *) gf_module_load(GF_VIDEO_OUTPUT_INTERFACE, gf_opts_get_key("core", "video-output") );

	if (!session->gl_driver) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Failed to load a video output for OpenGL context support !\n"));
		return GF_IO_ERR;
	}
	if (!gf_opts_get_key("core", "video-output")) {
		gf_opts_set_key("core", "video-output", session->gl_driver->module_name);
	}
	session->gl_driver->hw_caps |= GF_VIDEO_HW_INTERNAL;
	session->gl_driver->on_event = fsess_on_event;
	session->gl_driver->evt_cbk_hdl = session;

	os_disp_handler = NULL;
	sOpt = gf_opts_get_key("Temp", "OSDisp");
	if (sOpt) sscanf(sOpt, "%p", &os_disp_handler);

	e = session->gl_driver->Setup(session->gl_driver, NULL, os_disp_handler, GF_TERM_INIT_HIDE);
	if (e!=GF_OK) {
		GF_LOG(GF_LOG_WARNING, GF_LOG_FILTER, ("Failed to setup Video Driver %s!\n", session->gl_driver->module_name));
		gf_modules_close_interface((GF_BaseInterface *)session->gl_driver);
		session->gl_driver = NULL;
		return e;
	}

	//and initialize GL context
	memset(&evt, 0, sizeof(GF_Event));
	evt.type = GF_EVENT_VIDEO_SETUP;
	evt.setup.width = 128;
	evt.setup.height = 128;
	evt.setup.use_opengl = GF_TRUE;
	evt.setup.back_buffer = 1;
	//we anyway should'nt call swapBuffer/flush on this object
	evt.setup.disable_vsync = GF_TRUE;
	session->gl_driver->ProcessEvent(session->gl_driver, &evt);

	if (evt.setup.use_opengl) {
		gf_opengl_init();
	}
	return GF_OK;
}