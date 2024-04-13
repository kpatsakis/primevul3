evdns_base_nameserver_ip_add(struct evdns_base *base, const char *ip_as_string) {
	struct sockaddr_storage ss;
	struct sockaddr *sa;
	int len = sizeof(ss);
	int res;
	if (evutil_parse_sockaddr_port(ip_as_string, (struct sockaddr *)&ss,
		&len)) {
		log(EVDNS_LOG_WARN, "Unable to parse nameserver address %s",
			ip_as_string);
		return 4;
	}
	sa = (struct sockaddr *) &ss;
	if (sockaddr_getport(sa) == 0)
		sockaddr_setport(sa, 53);

	EVDNS_LOCK(base);
	res = evdns_nameserver_add_impl_(base, sa, len);
	EVDNS_UNLOCK(base);
	return res;
}
