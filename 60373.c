evdns_base_load_hosts(struct evdns_base *base, const char *hosts_fname)
{
	int res;
	if (!base)
		base = current_base;
	EVDNS_LOCK(base);
	res = evdns_base_load_hosts_impl(base, hosts_fname);
	EVDNS_UNLOCK(base);
	return res;
}
