evdns_base_load_hosts_impl(struct evdns_base *base, const char *hosts_fname)
{
	char *str=NULL, *cp, *eol;
	size_t len;
	int err=0;

	ASSERT_LOCKED(base);

	if (hosts_fname == NULL ||
	    (err = evutil_read_file_(hosts_fname, &str, &len, 0)) < 0) {
		char tmp[64];
		strlcpy(tmp, "127.0.0.1   localhost", sizeof(tmp));
		evdns_base_parse_hosts_line(base, tmp);
		strlcpy(tmp, "::1   localhost", sizeof(tmp));
		evdns_base_parse_hosts_line(base, tmp);
		return err ? -1 : 0;
	}

	/* This will break early if there is a NUL in the hosts file.
	 * Probably not a problem.*/
	cp = str;
	for (;;) {
		eol = strchr(cp, '\n');

		if (eol) {
			*eol = '\0';
			evdns_base_parse_hosts_line(base, cp);
			cp = eol+1;
		} else {
			evdns_base_parse_hosts_line(base, cp);
			break;
		}
	}

	mm_free(str);
	return 0;
}
