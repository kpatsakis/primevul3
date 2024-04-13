GetPinholePackets(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<PinholePackets>%u</PinholePackets>"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	struct NameValueParserData data;
	const char * uid_str;
	int n;
	char iaddr[INET6_ADDRSTRLEN];
	unsigned short iport;
	unsigned int packets = 0;
	int uid;
	int proto;
	unsigned int leasetime;

	if(CheckStatus(h)==0)
		return;

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	uid_str = GetValueFromNameValueList(&data, "UniqueID");
	uid = uid_str ? atoi(uid_str) : -1;
	ClearNameValueList(&data);

	if(uid < 0 || uid > 65535)
	{
		SoapError(h, 402, "Invalid Args");
		return;
	}

	/* Check that client is not getting infos of a pinhole
	 * it doesn't have access to, because of its public access */
	n = upnp_get_pinhole_info(uid, NULL, 0, NULL,
	                          iaddr, sizeof(iaddr), &iport,
	                          &proto,
	                          NULL, 0, /* desc, desclen */
	                          &leasetime, &packets);
	if (n >= 0)
	{
		if(PinholeVerification(h, iaddr, iport)<=0)
			return ;
	}
#if 0
	else if(r == -4 || r == -1)
	{
		SoapError(h, 704, "NoSuchEntry");
	}
#endif

	bodylen = snprintf(body, sizeof(body), resp,
			action, ns/*"urn:schemas-upnp-org:service:WANIPv6FirewallControl:1"*/,
			packets, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
