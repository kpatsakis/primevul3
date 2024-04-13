evdns_base_resolv_conf_parse(struct evdns_base *base, int flags, const char *const filename) {
	int res;
	EVDNS_LOCK(base);
	res = evdns_base_resolv_conf_parse_impl(base, flags, filename);
	EVDNS_UNLOCK(base);
	return res;
}
