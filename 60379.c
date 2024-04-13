evdns_base_resolv_conf_parse_impl(struct evdns_base *base, int flags, const char *const filename) {
	size_t n;
	char *resolv;
	char *start;
	int err = 0;

	log(EVDNS_LOG_DEBUG, "Parsing resolv.conf file %s", filename);

	if (flags & DNS_OPTION_HOSTSFILE) {
		char *fname = evdns_get_default_hosts_filename();
		evdns_base_load_hosts(base, fname);
		if (fname)
			mm_free(fname);
	}

	if ((err = evutil_read_file_(filename, &resolv, &n, 0)) < 0) {
		if (err == -1) {
			/* No file. */
			evdns_resolv_set_defaults(base, flags);
			return 1;
		} else {
			return 2;
		}
	}

	start = resolv;
	for (;;) {
		char *const newline = strchr(start, '\n');
		if (!newline) {
			resolv_conf_parse_line(base, start, flags);
			break;
		} else {
			*newline = 0;
			resolv_conf_parse_line(base, start, flags);
			start = newline + 1;
		}
	}

	if (!base->server_head && (flags & DNS_OPTION_NAMESERVERS)) {
		/* no nameservers were configured. */
		evdns_base_nameserver_ip_add(base, "127.0.0.1");
		err = 6;
	}
	if (flags & DNS_OPTION_SEARCH && (!base->global_search_state || base->global_search_state->num_domains == 0)) {
		search_set_from_hostname(base);
	}

	mm_free(resolv);
	return err;
}
