evdns_err_to_getaddrinfo_err(int e1)
{
	/* XXX Do this better! */
	if (e1 == DNS_ERR_NONE)
		return 0;
	else if (e1 == DNS_ERR_NOTEXIST)
		return EVUTIL_EAI_NONAME;
	else
		return EVUTIL_EAI_FAIL;
}
