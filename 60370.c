evdns_base_free(struct evdns_base *base, int fail_requests)
{
	EVDNS_LOCK(base);
	evdns_base_free_and_unlock(base, fail_requests);
}
