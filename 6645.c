GF_Err gf_filter_remove_event_listener(GF_Filter *filter, GF_FSEventListener *el)
{
	if (!filter || !filter->session || !el || !filter->session->event_listeners) return GF_BAD_PARAM;

	while (filter->session->in_event_listener) gf_sleep(1);
	gf_mx_p(filter->session->evt_mx);
	gf_list_del_item(filter->session->event_listeners, el);
	if (!gf_list_count(filter->session->event_listeners)) {
		gf_list_del(filter->session->event_listeners);
		filter->session->event_listeners=NULL;
	}
	gf_mx_v(filter->session->evt_mx);
	return GF_OK;
}