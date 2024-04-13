static Bool fs_default_event_proc(void *ptr, GF_Event *evt)
{
	GF_FilterSession *fs = (GF_FilterSession *)ptr;
	if (evt->type==GF_EVENT_QUIT) {
		gf_fs_abort(fs, GF_FS_FLUSH_FAST);
	}
	if (evt->type==GF_EVENT_MESSAGE) {
		if (evt->message.error) {
			if (evt->message.service) {
				GF_LOG(GF_LOG_ERROR, GF_LOG_AUTHOR, ("Service %s %s: %s\n", evt->message.service, evt->message.message, gf_error_to_string(evt->message.error) ))
			} else {
				GF_LOG(GF_LOG_ERROR, GF_LOG_AUTHOR, ("%s: %s\n", evt->message.message, gf_error_to_string(evt->message.error) ))
			}
		}
	}

#ifdef GPAC_HAS_QJS
	if (fs->on_evt_task)
		return jsfs_on_event(fs, evt);
#endif
	return 0;
}