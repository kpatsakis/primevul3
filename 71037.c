GetGenericPortMappingEntry(struct upnphttp * h, const char * action, const char * ns)
{
	int r;

	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewRemoteHost>%s</NewRemoteHost>"
		"<NewExternalPort>%u</NewExternalPort>"
		"<NewProtocol>%s</NewProtocol>"
		"<NewInternalPort>%u</NewInternalPort>"
		"<NewInternalClient>%s</NewInternalClient>"
		"<NewEnabled>1</NewEnabled>"
		"<NewPortMappingDescription>%s</NewPortMappingDescription>"
		"<NewLeaseDuration>%u</NewLeaseDuration>"
		"</u:%sResponse>";

	long int index = 0;
	unsigned short eport, iport;
	const char * m_index;
	char * endptr;
	char protocol[8], iaddr[32];
	char desc[64];
	char rhost[40];
	unsigned int leaseduration = 0;
	struct NameValueParserData data;

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	m_index = GetValueFromNameValueList(&data, "NewPortMappingIndex");

	if(!m_index)
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}
	errno = 0;	/* To distinguish success/failure after call */
	index = strtol(m_index, &endptr, 10);
	if((errno == ERANGE && (index == LONG_MAX || index == LONG_MIN))
	   || (errno != 0 && index == 0) || (m_index == endptr))
	{
		/* should condition (*endptr != '\0') be also an error ? */
		if(m_index == endptr)
			syslog(LOG_WARNING, "%s: no digits were found in <%s>",
			       "GetGenericPortMappingEntry", "NewPortMappingIndex");
		else
			syslog(LOG_WARNING, "%s: strtol('%s'): %m",
			       "GetGenericPortMappingEntry", m_index);
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}

	syslog(LOG_INFO, "%s: index=%d", action, (int)index);

	rhost[0] = '\0';
	r = upnp_get_redirection_infos_by_index((int)index, &eport, protocol, &iport,
                                            iaddr, sizeof(iaddr),
	                                        desc, sizeof(desc),
	                                        rhost, sizeof(rhost),
	                                        &leaseduration);

	if(r < 0)
	{
		SoapError(h, 713, "SpecifiedArrayIndexInvalid");
	}
	else
	{
		int bodylen;
		char body[2048];
		bodylen = snprintf(body, sizeof(body), resp,
			action, ns, /*SERVICE_TYPE_WANIPC,*/ rhost,
			(unsigned int)eport, protocol, (unsigned int)iport, iaddr, desc,
		    leaseduration, action);
		BuildSendAndCloseSoapResp(h, body, bodylen);
	}

	ClearNameValueList(&data);
}
