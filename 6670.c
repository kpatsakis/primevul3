GF_Err gf_filter_add_event_listener(GF_Filter *filter, GF_FSEventListener *el)
{
	GF_Err e;
	if (!filter || !filter->session || !el || !el->on_event) return GF_BAD_PARAM;
	while (filter->session->in_event_listener) gf_sleep(1);
	gf_mx_p(filter->session->evt_mx);
	if (!filter->session->event_listeners) {
		filter->session->event_listeners = gf_list_new();
	}
	e = gf_list_add(filter->session->event_listeners, el);
	gf_mx_v(filter->session->evt_mx);
	return e;
}