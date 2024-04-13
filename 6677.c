Bool gf_fs_forward_gf_event(GF_FilterSession *fsess, GF_Event *evt, Bool consumed, Bool skip_user)
{
	if (!fsess || fsess->in_final_flush) return GF_FALSE;

	if (fsess->event_listeners) {
		GF_FSEventListener *el;
		u32 i=0;

		gf_mx_p(fsess->evt_mx);
		fsess->in_event_listener ++;
		gf_mx_v(fsess->evt_mx);
		while ((el = gf_list_enum(fsess->event_listeners, &i))) {
			if (el->on_event(el->udta, evt, consumed)) {
				fsess->in_event_listener --;
				return GF_TRUE;
			}
		}
		fsess->in_event_listener --;
	}

	if (!skip_user && !consumed && fsess->ui_event_proc) {
		Bool res;
		res = gf_fs_ui_event(fsess, evt);
		return res;
	}
	return GF_FALSE;
}