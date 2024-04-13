evdns_resolv_set_defaults(struct evdns_base *base, int flags) {
	/* if the file isn't found then we assume a local resolver */
	ASSERT_LOCKED(base);
	if (flags & DNS_OPTION_SEARCH) search_set_from_hostname(base);
	if (flags & DNS_OPTION_NAMESERVERS) evdns_base_nameserver_ip_add(base,"127.0.0.1");
}
