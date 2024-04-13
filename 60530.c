evutil_set_evdns_getaddrinfo_fn_(evdns_getaddrinfo_fn fn)
{
	if (!evdns_getaddrinfo_impl)
		evdns_getaddrinfo_impl = fn;
}
