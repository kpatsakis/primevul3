SendSSDPMSEARCHResponse(int s, const struct sockaddr * sockname,
                        const char * st, const char * usn,
                        const char * server, const char * location)
{
	int l, n;
	char buf[512];
	socklen_t sockname_len;
	/*
	 * follow guideline from document "UPnP Device Architecture 1.0"
	 * uppercase is recommended.
	 * DATE: is recommended
	 * SERVER: OS/ver UPnP/1.0 miniupnpd/1.0
	 * - check what to put in the 'Cache-Control' header
	 *
	 * have a look at the document "UPnP Device Architecture v1.1 */
	l = snprintf(buf, sizeof(buf), "HTTP/1.1 200 OK\r\n"
		"CACHE-CONTROL: max-age=120\r\n"
		/*"DATE: ...\r\n"*/
		"ST: %s\r\n"
		"USN: %s\r\n"
		"EXT:\r\n"
		"SERVER: %s\r\n"
		"LOCATION: %s\r\n"
		"OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n" /* UDA v1.1 */
		"01-NLS: %u\r\n" /* same as BOOTID. UDA v1.1 */
		"BOOTID.UPNP.ORG: %u\r\n" /* UDA v1.1 */
		"CONFIGID.UPNP.ORG: %u\r\n" /* UDA v1.1 */
		"\r\n",
		st, usn,
		server, location,
		upnp_bootid, upnp_bootid, upnp_configid);
#ifdef ENABLE_IPV6
	sockname_len = (sockname->sa_family == PF_INET6)
	             ? sizeof(struct sockaddr_in6)
	             : sizeof(struct sockaddr_in);
#else	/* ENABLE_IPV6 */
	sockname_len = sizeof(struct sockaddr_in);
#endif	/* ENABLE_IPV6 */
	n = sendto_or_schedule(s, buf, l, 0, sockname, sockname_len);
	if(n < 0) {
		syslog(LOG_ERR, "%s: sendto(udp): %m", __func__);
	}
}
