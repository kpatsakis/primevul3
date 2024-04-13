DeletePinhole(struct upnphttp * h, const char * action, const char * ns)
{
	int n;
#if 0
	static const char resp[] =
		"<u:DeletePinholeResponse "
		"xmlns:u=\"urn:schemas-upnp-org:service:WANIPv6FirewallControl:1\">"
		"</u:DeletePinholeResponse>";
#endif
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"</u:%sResponse>";
	char body[512];
	int bodylen;

	struct NameValueParserData data;
	const char * uid_str;
	char iaddr[INET6_ADDRSTRLEN];
	int proto;
	unsigned short iport;
	unsigned int leasetime;
	int uid;

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

	/* Check that client is not deleting an pinhole
	 * it doesn't have access to, because of its public access */
	n = upnp_get_pinhole_info(uid, NULL, 0, NULL,
	                          iaddr, sizeof(iaddr), &iport,
	                          &proto,
	                          NULL, 0, /* desc, desclen */
	                          &leasetime, NULL);
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

	n = upnp_delete_inboundpinhole(uid);
	if(n < 0)
	{
		syslog(LOG_INFO, "%s: (inbound) failed to remove pinhole with ID: %d",
	           action, uid);
		SoapError(h, 501, "ActionFailed");
		return;
	}
	syslog(LOG_INFO, "%s: (inbound) pinhole with ID %d successfully removed",
	       action, uid);
	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
