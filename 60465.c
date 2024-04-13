resolv_conf_parse_line(struct evdns_base *base, char *const start, int flags) {
	char *strtok_state;
	static const char *const delims = " \t";
#define NEXT_TOKEN strtok_r(NULL, delims, &strtok_state)


	char *const first_token = strtok_r(start, delims, &strtok_state);
	ASSERT_LOCKED(base);
	if (!first_token) return;

	if (!strcmp(first_token, "nameserver") && (flags & DNS_OPTION_NAMESERVERS)) {
		const char *const nameserver = NEXT_TOKEN;

		if (nameserver)
			evdns_base_nameserver_ip_add(base, nameserver);
	} else if (!strcmp(first_token, "domain") && (flags & DNS_OPTION_SEARCH)) {
		const char *const domain = NEXT_TOKEN;
		if (domain) {
			search_postfix_clear(base);
			search_postfix_add(base, domain);
		}
	} else if (!strcmp(first_token, "search") && (flags & DNS_OPTION_SEARCH)) {
		const char *domain;
		search_postfix_clear(base);

		while ((domain = NEXT_TOKEN)) {
			search_postfix_add(base, domain);
		}
		search_reverse(base);
	} else if (!strcmp(first_token, "options")) {
		const char *option;
		while ((option = NEXT_TOKEN)) {
			const char *val = strchr(option, ':');
			evdns_base_set_option_impl(base, option, val ? val+1 : "", flags);
		}
	}
#undef NEXT_TOKEN
}
