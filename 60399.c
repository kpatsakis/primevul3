evdns_getaddrinfo_cancel(struct evdns_getaddrinfo_request *data)
{
	EVDNS_LOCK(data->evdns_base);
	if (data->request_done) {
		EVDNS_UNLOCK(data->evdns_base);
		return;
	}
	event_del(&data->timeout);
	data->user_canceled = 1;
	if (data->ipv4_request.r)
		evdns_cancel_request(data->evdns_base, data->ipv4_request.r);
	if (data->ipv6_request.r)
		evdns_cancel_request(data->evdns_base, data->ipv6_request.r);
	EVDNS_UNLOCK(data->evdns_base);
}
