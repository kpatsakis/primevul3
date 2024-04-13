SetDefaultConnectionService(struct upnphttp * h, const char * action, const char * ns)
{
	/*static const char resp[] =
		"<u:SetDefaultConnectionServiceResponse "
		"xmlns:u=\"urn:schemas-upnp-org:service:Layer3Forwarding:1\">"
		"</u:SetDefaultConnectionServiceResponse>";*/
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	struct NameValueParserData data;
	char * p;
	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	p = GetValueFromNameValueList(&data, "NewDefaultConnectionService");
	if(p) {
		/* 720 InvalidDeviceUUID
		 * 721 InvalidServiceID
		 * 723 InvalidConnServiceSelection */
#ifdef UPNP_STRICT
		char * service;
		service = strchr(p, ',');
		if(0 != memcmp(uuidvalue_wcd, p, sizeof("uuid:00000000-0000-0000-0000-000000000000") - 1)) {
			SoapError(h, 720, "InvalidDeviceUUID");
		} else if(service == NULL || 0 != strcmp(service+1, SERVICE_ID_WANIPC)) {
			SoapError(h, 721, "InvalidServiceID");
		} else
#endif
		{
			syslog(LOG_INFO, "%s(%s) : Ignored", action, p);
			bodylen = snprintf(body, sizeof(body), resp,
			                   action, ns, action);
			BuildSendAndCloseSoapResp(h, body, bodylen);
		}
	} else {
		/* missing argument */
		SoapError(h, 402, "Invalid Args");
	}
	ClearNameValueList(&data);
}
