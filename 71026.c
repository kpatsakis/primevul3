CheckPinholeWorking(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<IsWorking>%d</IsWorking>"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	int r;
	struct NameValueParserData data;
	const char * uid_str;
	int uid;
	char iaddr[INET6_ADDRSTRLEN];
	unsigned short iport;
	unsigned int packets;

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

	/* Check that client is not checking a pinhole
	 * it doesn't have access to, because of its public access */
	r = upnp_get_pinhole_info(uid,
	                          NULL, 0, NULL,
	                          iaddr, sizeof(iaddr), &iport,
	                          NULL, /* proto */
	                          NULL, 0, /* desc, desclen */
	                          NULL, &packets);
	if (r >= 0)
	{
		if(PinholeVerification(h, iaddr, iport) <= 0)
			return ;
		if(packets == 0)
		{
			SoapError(h, 709, "NoPacketSent");
			return;
		}
		bodylen = snprintf(body, sizeof(body), resp,
						action, ns/*"urn:schemas-upnp-org:service:WANIPv6FirewallControl:1"*/,
						1, action);
		BuildSendAndCloseSoapResp(h, body, bodylen);
	}
	else if(r == -2)
		SoapError(h, 704, "NoSuchEntry");
	else
		SoapError(h, 501, "ActionFailed");
}
