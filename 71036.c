GetExternalIPAddress(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<NewExternalIPAddress>%s</NewExternalIPAddress>"
		"</u:%sResponse>";

	char body[512];
	int bodylen;
	char ext_ip_addr[INET_ADDRSTRLEN];
	/* Does that method need to work with IPv6 ?
	 * There is usually no NAT with IPv6 */

#ifndef MULTIPLE_EXTERNAL_IP
	struct in_addr addr;
	if(use_ext_ip_addr)
	{
		strncpy(ext_ip_addr, use_ext_ip_addr, INET_ADDRSTRLEN);
		ext_ip_addr[INET_ADDRSTRLEN - 1] = '\0';
	}
	else if(getifaddr(ext_if_name, ext_ip_addr, INET_ADDRSTRLEN, &addr, NULL) < 0)
	{
		syslog(LOG_ERR, "Failed to get ip address for interface %s",
			ext_if_name);
		strncpy(ext_ip_addr, "0.0.0.0", INET_ADDRSTRLEN);
	}
	if (addr_is_reserved(&addr))
		strncpy(ext_ip_addr, "0.0.0.0", INET_ADDRSTRLEN);
#else
	struct lan_addr_s * lan_addr;
	strncpy(ext_ip_addr, "0.0.0.0", INET_ADDRSTRLEN);
	for(lan_addr = lan_addrs.lh_first; lan_addr != NULL; lan_addr = lan_addr->list.le_next)
	{
		if( (h->clientaddr.s_addr & lan_addr->mask.s_addr)
		   == (lan_addr->addr.s_addr & lan_addr->mask.s_addr))
		{
			strncpy(ext_ip_addr, lan_addr->ext_ip_str, INET_ADDRSTRLEN);
			break;
		}
	}
#endif
	if (strcmp(ext_ip_addr, "0.0.0.0") == 0)
	{
		SoapError(h, 501, "Action Failed");
		return;
	}
	bodylen = snprintf(body, sizeof(body), resp,
	              action, ns, /*SERVICE_TYPE_WANIPC,*/
				  ext_ip_addr, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
