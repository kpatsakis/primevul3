evdns_base_resume(struct evdns_base *base)
{
	EVDNS_LOCK(base);
	evdns_requests_pump_waiting_queue(base);
	EVDNS_UNLOCK(base);

	return 0;
}
