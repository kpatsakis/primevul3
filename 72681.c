void send_open_url(const char *url)
{
	GF_Event evt;
	memset(&evt, 0, sizeof(GF_Event));
	evt.type = GF_EVENT_NAVIGATE;
	evt.navigate.to_url = url;
	gf_term_send_event(term, &evt);
}
