DeletePortMappingRange(struct upnphttp * h, const char * action, const char * ns)
{
	int r = -1;
	/*static const char resp[] =
		"<u:DeletePortMappingRangeResponse "
		"xmlns:u=\"" SERVICE_TYPE_WANIPC "\">"
		"</u:DeletePortMappingRangeResponse>";*/
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	struct NameValueParserData data;
	const char * protocol;
	const char * startport_s, * endport_s;
	unsigned short startport, endport;
	/*int manage;*/
	unsigned short * port_list;
	unsigned int i, number = 0;

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	startport_s = GetValueFromNameValueList(&data, "NewStartPort");
	endport_s = GetValueFromNameValueList(&data, "NewEndPort");
	protocol = GetValueFromNameValueList(&data, "NewProtocol");
	/*manage = atoi(GetValueFromNameValueList(&data, "NewManage"));*/
	if(startport_s == NULL || endport_s == NULL || protocol == NULL ||
	   !is_numeric(startport_s) || !is_numeric(endport_s)) {
		SoapError(h, 402, "Invalid Args");
		ClearNameValueList(&data);
		return;
	}
	startport = (unsigned short)atoi(startport_s);
	endport = (unsigned short)atoi(endport_s);

	/* possible errors :
	   606 - Action not authorized
	   730 - PortMappingNotFound
	   733 - InconsistentParameter
	 */
	if(startport > endport)
	{
		SoapError(h, 733, "InconsistentParameter");
		ClearNameValueList(&data);
		return;
	}

	syslog(LOG_INFO, "%s: deleting external ports: %hu-%hu, protocol: %s",
	       action, startport, endport, protocol);

	port_list = upnp_get_portmappings_in_range(startport, endport,
	                                           protocol, &number);
	if(number == 0)
	{
		SoapError(h, 730, "PortMappingNotFound");
		ClearNameValueList(&data);
		free(port_list);
		return;
	}

	for(i = 0; i < number; i++)
	{
		r = upnp_delete_redirection(port_list[i], protocol);
		syslog(LOG_INFO, "%s: deleting external port: %hu, protocol: %s: %s",
		       action, port_list[i], protocol, r < 0 ? "failed" : "ok");
	}
	free(port_list);
	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);

	ClearNameValueList(&data);
}
