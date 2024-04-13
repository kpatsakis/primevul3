process_redirect_pdu(STREAM s, RD_BOOL enhanced_redirect /*, uint32 * ext_disc_reason */ )
{
	uint32 len;
	uint16 redirect_identifier;

	logger(Protocol, Debug, "%s()", __func__);

	/* reset any previous redirection information */
	g_redirect = True;
	free(g_redirect_server);
	free(g_redirect_username);
	free(g_redirect_domain);
	free(g_redirect_lb_info);
	free(g_redirect_cookie);

	g_redirect_server = NULL;
	g_redirect_username = NULL;
	g_redirect_domain = NULL;
	g_redirect_lb_info = NULL;
	g_redirect_cookie = NULL;

	/* these 2 bytes are unknown, seem to be zeros */
	in_uint8s(s, 2);

	/* FIXME: Previous implementation only reads 4 bytes which has been working
	   but todays spec says something different. Investigate and retest
	   server redirection using WTS 2003 cluster.
	 */

	if (enhanced_redirect)
	{
		/* read identifier */
		in_uint16_le(s, redirect_identifier);
		if (redirect_identifier != 0x0400)
			logger(Protocol, Error, "unexpected data in server redirection packet");

		/* FIXME: skip total length */
		in_uint8s(s, 2);

		/* read session_id */
		in_uint32_le(s, g_redirect_session_id);
	}

	/* read connection flags */
	in_uint32_le(s, g_redirect_flags);

	if (g_redirect_flags & LB_TARGET_NET_ADDRESS)
	{
		/* read length of ip string */
		in_uint32_le(s, len);

		/* read ip string */
		rdp_in_unistr(s, len, &g_redirect_server, &g_redirect_server_len);
	}

	if (g_redirect_flags & LB_LOAD_BALANCE_INFO)
	{
		/* read length of load balance info blob */
		in_uint32_le(s, g_redirect_lb_info_len);

		/* reallocate a loadbalance info blob */
		if (g_redirect_lb_info != NULL)
			free(g_redirect_lb_info);

		g_redirect_lb_info = xmalloc(g_redirect_lb_info_len);

		/* read load balance info blob */
		in_uint8p(s, g_redirect_lb_info, g_redirect_lb_info_len);
	}

	if (g_redirect_flags & LB_USERNAME)
	{
		/* read length of username string */
		in_uint32_le(s, len);

		/* read username string */
		rdp_in_unistr(s, len, &g_redirect_username, &g_redirect_username_len);
	}

	if (g_redirect_flags & LB_DOMAIN)
	{
		/* read length of domain string */
		in_uint32_le(s, len);

		/* read domain string */
		rdp_in_unistr(s, len, &g_redirect_domain, &g_redirect_domain_len);
	}

	if (g_redirect_flags & LB_PASSWORD)
	{
		/* the information in this blob is either a password or a cookie that
		   should be passed though as blob and not parsed as a unicode string */

		/* read blob length */
		in_uint32_le(s, g_redirect_cookie_len);

		/* reallocate cookie blob */
		if (g_redirect_cookie != NULL)
			free(g_redirect_cookie);

		g_redirect_cookie = xmalloc(g_redirect_cookie_len);

		/* read cookie as is */
		in_uint8p(s, g_redirect_cookie, g_redirect_cookie_len);

		logger(Protocol, Debug, "process_redirect_pdu(), Read %d bytes redirection cookie",
		       g_redirect_cookie_len);
	}

	if (g_redirect_flags & LB_DONTSTOREUSERNAME)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_DONTSTOREUSERNAME set");
	}

	if (g_redirect_flags & LB_SMARTCARD_LOGON)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_SMARTCARD_LOGON set");
	}

	if (g_redirect_flags & LB_NOREDIRECT)
	{
		/* By spec this is only for information and doesn't mean that an actual
		   redirect should be performed. How it should be used is not mentioned. */
		g_redirect = False;
	}

	if (g_redirect_flags & LB_TARGET_FQDN)
	{
		in_uint32_le(s, len);

		/* Let target FQDN replace target IP address */
		if (g_redirect_server)
		{
			free(g_redirect_server);
			g_redirect_server = NULL;
		}

		/* read FQDN string */
		rdp_in_unistr(s, len, &g_redirect_server, &g_redirect_server_len);
	}

	if (g_redirect_flags & LB_TARGET_NETBIOS)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_TARGET_NETBIOS");
	}

	if (g_redirect_flags & LB_TARGET_NET_ADDRESSES)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_TARGET_NET_ADDRESSES");
	}

	if (g_redirect_flags & LB_CLIENT_TSV_URL)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_CLIENT_TSV_URL");
	}

	if (g_redirect_flags & LB_SERVER_TSV_CAPABLE)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_SERVER_TSV_URL");
	}

	if (g_redirect_flags & LB_PASSWORD_IS_PK_ENCRYPTED)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_PASSWORD_IS_PK_ENCRYPTED ");
	}

	if (g_redirect_flags & LB_REDIRECTION_GUID)
	{
		logger(Protocol, Warning, "process_redirect_pdu(), unhandled LB_REDIRECTION_GUID ");
	}

	if (g_redirect_flags & LB_TARGET_CERTIFICATE)
	{
		logger(Protocol, Warning,
		       "process_redirect_pdu(), unhandled LB_TARGET_CERTIFICATE");
	}

	return g_redirect;
}
