evdns_request_timeout_callback(evutil_socket_t fd, short events, void *arg) {
	struct request *const req = (struct request *) arg;
	struct evdns_base *base = req->base;

	(void) fd;
	(void) events;

	log(EVDNS_LOG_DEBUG, "Request %p timed out", arg);
	EVDNS_LOCK(base);

	if (req->tx_count >= req->base->global_max_retransmits) {
		struct nameserver *ns = req->ns;
		/* this request has failed */
		log(EVDNS_LOG_DEBUG, "Giving up on request %p; tx_count==%d",
		    arg, req->tx_count);
		reply_schedule_callback(req, 0, DNS_ERR_TIMEOUT, NULL);

		request_finished(req, &REQ_HEAD(req->base, req->trans_id), 1);
		nameserver_failed(ns, "request timed out.");
	} else {
		/* retransmit it */
		log(EVDNS_LOG_DEBUG, "Retransmitting request %p; tx_count==%d",
		    arg, req->tx_count);
		(void) evtimer_del(&req->timeout_event);
		request_swap_ns(req, nameserver_pick(base));
		evdns_request_transmit(req);

		req->ns->timedout++;
		if (req->ns->timedout > req->base->global_max_nameserver_timeout) {
			req->ns->timedout = 0;
			nameserver_failed(req->ns, "request timed out.");
		}
	}

	EVDNS_UNLOCK(base);
}
