evdns_base_set_option(struct evdns_base *base,
    const char *option, const char *val)
{
	int res;
	EVDNS_LOCK(base);
	res = evdns_base_set_option_impl(base, option, val, DNS_OPTIONS_ALL);
	EVDNS_UNLOCK(base);
	return res;
}
