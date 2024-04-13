GetCommonLinkProperties(struct upnphttp * h, const char * action, const char * ns)
{
	/* WANAccessType : set depending on the hardware :
	 * DSL, POTS (plain old Telephone service), Cable, Ethernet */
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewWANAccessType>%s</NewWANAccessType>"
		"<NewLayer1UpstreamMaxBitRate>%lu</NewLayer1UpstreamMaxBitRate>"
		"<NewLayer1DownstreamMaxBitRate>%lu</NewLayer1DownstreamMaxBitRate>"
		"<NewPhysicalLinkStatus>%s</NewPhysicalLinkStatus>"
		"</u:%sResponse>";

	char body[2048];
	int bodylen;
	struct ifdata data;
	const char * status = "Up";	/* Up, Down (Required),
	                             * Initializing, Unavailable (Optional) */
	const char * wan_access_type = "Cable"; /* DSL, POTS, Cable, Ethernet */
	char ext_ip_addr[INET_ADDRSTRLEN];

	if((downstream_bitrate == 0) || (upstream_bitrate == 0))
	{
		if(getifstats(ext_if_name, &data) >= 0)
		{
			if(downstream_bitrate == 0) downstream_bitrate = data.baudrate;
			if(upstream_bitrate == 0) upstream_bitrate = data.baudrate;
		}
	}
	if(getifaddr(ext_if_name, ext_ip_addr, INET_ADDRSTRLEN, NULL, NULL) < 0) {
		status = "Down";
	}
	bodylen = snprintf(body, sizeof(body), resp,
	    action, ns, /* was "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1" */
	    wan_access_type,
	    upstream_bitrate, downstream_bitrate,
	    status, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
