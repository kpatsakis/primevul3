GetConnectionTypeInfo(struct upnphttp * h, const char * action, const char * ns)
{
#if 0
	static const char resp[] =
		"<u:GetConnectionTypeInfoResponse "
		"xmlns:u=\"" SERVICE_TYPE_WANIPC "\">"
		"<NewConnectionType>IP_Routed</NewConnectionType>"
		"<NewPossibleConnectionTypes>IP_Routed</NewPossibleConnectionTypes>"
		"</u:GetConnectionTypeInfoResponse>";
#endif
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewConnectionType>IP_Routed</NewConnectionType>"
		"<NewPossibleConnectionTypes>IP_Routed</NewPossibleConnectionTypes>"
		"</u:%sResponse>";
	char body[512];
	int bodylen;

	bodylen = snprintf(body, sizeof(body), resp,
	         action, ns, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
