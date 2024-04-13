nameserver_write_waiting(struct nameserver *ns, char waiting) {
	ASSERT_LOCKED(ns->base);
	if (ns->write_waiting == waiting) return;

	ns->write_waiting = waiting;
	(void) event_del(&ns->event);
	event_assign(&ns->event, ns->base->event_base,
	    ns->socket, EV_READ | (waiting ? EV_WRITE : 0) | EV_PERSIST,
	    nameserver_ready_callback, ns);
	if (event_add(&ns->event, NULL) < 0) {
		char addrbuf[128];
		log(EVDNS_LOG_WARN, "Error from libevent when adding event for %s",
		    evutil_format_sockaddr_port_(
			    (struct sockaddr *)&ns->address,
			    addrbuf, sizeof(addrbuf)));
		/* ???? Do more? */
	}
}
