evdns_nameserver_ip_add(const char *ip_as_string) {
	if (!current_base)
		current_base = evdns_base_new(NULL, 0);
	return evdns_base_nameserver_ip_add(current_base, ip_as_string);
}
