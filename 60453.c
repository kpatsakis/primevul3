nameserver_up(struct nameserver *const ns)
{
	char addrbuf[128];
	ASSERT_LOCKED(ns->base);
	if (ns->state) return;
	log(EVDNS_LOG_MSG, "Nameserver %s is back up",
	    evutil_format_sockaddr_port_(
		    (struct sockaddr *)&ns->address,
		    addrbuf, sizeof(addrbuf)));
	evtimer_del(&ns->timeout_event);
	if (ns->probe_request) {
		evdns_cancel_request(ns->base, ns->probe_request);
		ns->probe_request = NULL;
	}
	ns->state = 1;
	ns->failed_times = 0;
	ns->timedout = 0;
	ns->base->global_good_nameservers++;
}
