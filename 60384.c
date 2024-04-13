evdns_base_search_add(struct evdns_base *base, const char *domain) {
	EVDNS_LOCK(base);
	search_postfix_add(base, domain);
	EVDNS_UNLOCK(base);
}
