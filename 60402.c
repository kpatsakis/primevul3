evdns_getaddrinfo_set_timeout(struct evdns_base *evdns_base,
    struct evdns_getaddrinfo_request *data)
{
	return event_add(&data->timeout, &evdns_base->global_getaddrinfo_allow_skew);
}
