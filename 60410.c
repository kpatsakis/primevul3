evdns_nameserver_ip_add_line(struct evdns_base *base, const char *ips) {
	const char *addr;
	char *buf;
	int r;
	ASSERT_LOCKED(base);
	while (*ips) {
		while (isspace(*ips) || *ips == ',' || *ips == '\t')
			++ips;
		addr = ips;
		while (isdigit(*ips) || *ips == '.' || *ips == ':' ||
		    *ips=='[' || *ips==']')
			++ips;
		buf = mm_malloc(ips-addr+1);
		if (!buf) return 4;
		memcpy(buf, addr, ips-addr);
		buf[ips-addr] = '\0';
		r = evdns_base_nameserver_ip_add(base, buf);
		mm_free(buf);
		if (r) return r;
	}
	return 0;
}
