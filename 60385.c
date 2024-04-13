evdns_base_search_clear(struct evdns_base *base)
{
	EVDNS_LOCK(base);
	search_postfix_clear(base);
	EVDNS_UNLOCK(base);
}
