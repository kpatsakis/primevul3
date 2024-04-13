GetTotalPacketsSent(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewTotalPacketsSent>%lu</NewTotalPacketsSent>"
		"</u:%sResponse>";

	char body[512];
	int bodylen;
	struct ifdata data;

	r = getifstats(ext_if_name, &data);
	bodylen = snprintf(body, sizeof(body), resp,
	         action, ns,/*"urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",*/
#ifdef UPNP_STRICT
	         r<0?0:(data.opackets & UPNP_UI4_MAX), action);
#else /* UPNP_STRICT */
	         r<0?0:data.opackets, action);
#endif /* UPNP_STRICT */
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
