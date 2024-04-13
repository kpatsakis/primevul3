void gf_fs_set_ui_callback(GF_FilterSession *fs, Bool (*ui_event_proc)(void *opaque, GF_Event *event), void *cbk_udta)
{
	if (fs) {
		fs->ui_event_proc = ui_event_proc;
		fs->ui_opaque = cbk_udta;
		if (!fs->ui_event_proc) {
			fs->ui_event_proc = fs_default_event_proc;
			fs->ui_opaque = fs;
		}
	}
}