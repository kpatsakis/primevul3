UpdatePinhole(struct upnphttp * h, const char * action, const char * ns)
{
#if 0
	static const char resp[] =
		"<u:UpdatePinholeResponse "
		"xmlns:u=\"urn:schemas-upnp-org:service:WANIPv6FirewallControl:1\">"
		"</u:UpdatePinholeResponse>";
#endif
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"</u:%sResponse>";
	char body[512];
	int bodylen;
	struct NameValueParserData data;
	const char * uid_str, * leaseTime;
	char iaddr[INET6_ADDRSTRLEN];
	unsigned short iport;
	int ltime;
	int uid;
	int n;

	if(CheckStatus(h)==0)
		return;

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	uid_str = GetValueFromNameValueList(&data, "UniqueID");
	leaseTime = GetValueFromNameValueList(&data, "NewLeaseTime");
	uid = uid_str ? atoi(uid_str) : -1;
	ltime = leaseTime ? atoi(leaseTime) : -1;
	ClearNameValueList(&data);

	if(uid < 0 || uid > 65535 || ltime <= 0 || ltime > 86400)
	{
		SoapError(h, 402, "Invalid Args");
		return;
	}

	/* Check that client is not updating an pinhole
	 * it doesn't have access to, because of its public access */
	n = upnp_get_pinhole_info(uid, NULL, 0, NULL,
	                          iaddr, sizeof(iaddr), &iport,
	                          NULL, /* proto */
	                          NULL, 0, /* desc, desclen */
	                          NULL, NULL);
	if (n >= 0)
	{
		if(PinholeVerification(h, iaddr, iport) <= 0)
			return;
	}
	else if(n == -2)
	{
		SoapError(h, 704, "NoSuchEntry");
		return;
	}
	else
	{
		SoapError(h, 501, "ActionFailed");
		return;
	}

	syslog(LOG_INFO, "%s: (inbound) updating lease duration to %d for pinhole with ID: %d",
	       action, ltime, uid);

	n = upnp_update_inboundpinhole(uid, ltime);
	if(n == -1)
		SoapError(h, 704, "NoSuchEntry");
	else if(n < 0)
		SoapError(h, 501, "ActionFailed");
	else {
		bodylen = snprintf(body, sizeof(body), resp,
		                   action, ns, action);
		BuildSendAndCloseSoapResp(h, body, bodylen);
	}
}
