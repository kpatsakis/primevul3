GetTotalPacketsReceived(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewTotalPacketsReceived>%lu</NewTotalPacketsReceived>"
		"</u:%sResponse>";

	char body[512];
	int bodylen;
	struct ifdata data;

	r = getifstats(ext_if_name, &data);
	bodylen = snprintf(body, sizeof(body), resp,
	         action, ns, /* was "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1" */
#ifdef UPNP_STRICT
	         r<0?0:(data.ipackets & UPNP_UI4_MAX), action);
#else /* UPNP_STRICT */
	         r<0?0:data.ipackets, action);
#endif /* UPNP_STRICT */
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
