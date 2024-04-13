GetListOfPortMappings(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp_start[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewPortListing><![CDATA[";
	static const char resp_end[] =
		"]]></NewPortListing>"
		"</u:%sResponse>";

	static const char list_start[] =
		"<p:PortMappingList xmlns:p=\"urn:schemas-upnp-org:gw:WANIPConnection\""
		" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
		" xsi:schemaLocation=\"urn:schemas-upnp-org:gw:WANIPConnection"
		" http://www.upnp.org/schemas/gw/WANIPConnection-v2.xsd\">";
	static const char list_end[] =
		"</p:PortMappingList>";

	static const char entry[] =
		"<p:PortMappingEntry>"
		"<p:NewRemoteHost>%s</p:NewRemoteHost>"
		"<p:NewExternalPort>%hu</p:NewExternalPort>"
		"<p:NewProtocol>%s</p:NewProtocol>"
		"<p:NewInternalPort>%hu</p:NewInternalPort>"
		"<p:NewInternalClient>%s</p:NewInternalClient>"
		"<p:NewEnabled>1</p:NewEnabled>"
		"<p:NewDescription>%s</p:NewDescription>"
		"<p:NewLeaseTime>%u</p:NewLeaseTime>"
		"</p:PortMappingEntry>";

	char * body;
	size_t bodyalloc;
	int bodylen;

	int r = -1;
	unsigned short iport;
	char int_ip[32];
	char desc[64];
	char rhost[64];
	unsigned int leaseduration = 0;

	struct NameValueParserData data;
	const char * startport_s, * endport_s;
	unsigned short startport, endport;
	const char * protocol;
	/*int manage;*/
	const char * number_s;
	int number;
	unsigned short * port_list;
	unsigned int i, list_size = 0;

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	startport_s = GetValueFromNameValueList(&data, "NewStartPort");
	endport_s = GetValueFromNameValueList(&data, "NewEndPort");
	protocol = GetValueFromNameValueList(&data, "NewProtocol");
	/*manage_s = GetValueFromNameValueList(&data, "NewManage");*/
	number_s = GetValueFromNameValueList(&data, "NewNumberOfPorts");
	if(startport_s == NULL || endport_s == NULL || protocol == NULL ||
	   number_s == NULL || !is_numeric(number_s) ||
	   !is_numeric(startport_s) || !is_numeric(endport_s)) {
		SoapError(h, 402, "Invalid Args");
		ClearNameValueList(&data);
		return;
	}

	startport = (unsigned short)atoi(startport_s);
	endport = (unsigned short)atoi(endport_s);
	/*manage = atoi(manage_s);*/
	number = atoi(number_s);
	if(number == 0) number = 1000;	/* return up to 1000 mappings by default */

	if(startport > endport)
	{
		SoapError(h, 733, "InconsistentParameter");
		ClearNameValueList(&data);
		return;
	}
/*
build the PortMappingList xml document :

<p:PortMappingList xmlns:p="urn:schemas-upnp-org:gw:WANIPConnection"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="urn:schemas-upnp-org:gw:WANIPConnection
http://www.upnp.org/schemas/gw/WANIPConnection-v2.xsd">
<p:PortMappingEntry>
<p:NewRemoteHost>202.233.2.1</p:NewRemoteHost>
<p:NewExternalPort>2345</p:NewExternalPort>
<p:NewProtocol>TCP</p:NewProtocol>
<p:NewInternalPort>2345</p:NewInternalPort>
<p:NewInternalClient>192.168.1.137</p:NewInternalClient>
<p:NewEnabled>1</p:NewEnabled>
<p:NewDescription>dooom</p:NewDescription>
<p:NewLeaseTime>345</p:NewLeaseTime>
</p:PortMappingEntry>
</p:PortMappingList>
*/
	bodyalloc = 4096;
	body = malloc(bodyalloc);
	if(!body)
	{
		ClearNameValueList(&data);
		SoapError(h, 501, "ActionFailed");
		return;
	}
	bodylen = snprintf(body, bodyalloc, resp_start,
	              action, ns/*SERVICE_TYPE_WANIPC*/);
	if(bodylen < 0)
	{
		SoapError(h, 501, "ActionFailed");
		free(body);
		return;
	}
	memcpy(body+bodylen, list_start, sizeof(list_start));
	bodylen += (sizeof(list_start) - 1);

	port_list = upnp_get_portmappings_in_range(startport, endport,
	                                           protocol, &list_size);
	/* loop through port mappings */
	for(i = 0; number > 0 && i < list_size; i++)
	{
		/* have a margin of 1024 bytes to store the new entry */
		if((unsigned int)bodylen + 1024 > bodyalloc)
		{
			char * body_sav = body;
			bodyalloc += 4096;
			body = realloc(body, bodyalloc);
			if(!body)
			{
				syslog(LOG_CRIT, "realloc(%p, %u) FAILED", body_sav, (unsigned)bodyalloc);
				ClearNameValueList(&data);
				SoapError(h, 501, "ActionFailed");
				free(body_sav);
				free(port_list);
				return;
			}
		}
		rhost[0] = '\0';
		r = upnp_get_redirection_infos(port_list[i], protocol, &iport,
		                               int_ip, sizeof(int_ip),
		                               desc, sizeof(desc),
		                               rhost, sizeof(rhost),
		                               &leaseduration);
		if(r == 0)
		{
			bodylen += snprintf(body+bodylen, bodyalloc-bodylen, entry,
			                    rhost, port_list[i], protocol,
			                    iport, int_ip, desc, leaseduration);
			number--;
		}
	}
	free(port_list);
	port_list = NULL;

	if((bodylen + sizeof(list_end) + 1024) > bodyalloc)
	{
		char * body_sav = body;
		bodyalloc += (sizeof(list_end) + 1024);
		body = realloc(body, bodyalloc);
		if(!body)
		{
			syslog(LOG_CRIT, "realloc(%p, %u) FAILED", body_sav, (unsigned)bodyalloc);
			ClearNameValueList(&data);
			SoapError(h, 501, "ActionFailed");
			free(body_sav);
			return;
		}
	}
	memcpy(body+bodylen, list_end, sizeof(list_end));
	bodylen += (sizeof(list_end) - 1);
	bodylen += snprintf(body+bodylen, bodyalloc-bodylen, resp_end,
	                    action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
	free(body);

	ClearNameValueList(&data);
}
