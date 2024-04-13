GetTotalBytesReceived(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewTotalBytesReceived>%lu</NewTotalBytesReceived>"
		"</u:%sResponse>";

	char body[512];
	int bodylen;
	struct ifdata data;

	r = getifstats(ext_if_name, &data);
	/* TotalBytesReceived
	 * This variable represents the cumulative counter for total number of
	 * bytes received downstream across all connection service instances on
	 * WANDevice. The count rolls over to 0 after it reaching the maximum
	 * value (2^32)-1. */
	bodylen = snprintf(body, sizeof(body), resp,
	         action, ns, /* was "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1" */
#ifdef UPNP_STRICT
	         r<0?0:(data.ibytes & UPNP_UI4_MAX), action);
#else /* UPNP_STRICT */
	         r<0?0:data.ibytes, action);
#endif /* UPNP_STRICT */
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
