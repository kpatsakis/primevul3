evdns_request_transmit(struct request *req) {
	int retcode = 0, r;

	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);
	/* if we fail to send this packet then this flag marks it */
	/* for evdns_transmit */
	req->transmit_me = 1;
	EVUTIL_ASSERT(req->trans_id != 0xffff);

	if (!req->ns)
	{
		/* unable to transmit request if no nameservers */
		return 1;
	}

	if (req->ns->choked) {
		/* don't bother trying to write to a socket */
		/* which we have had EAGAIN from */
		return 1;
	}

	r = evdns_request_transmit_to(req, req->ns);
	switch (r) {
	case 1:
		/* temp failure */
		req->ns->choked = 1;
		nameserver_write_waiting(req->ns, 1);
		return 1;
	case 2:
		/* failed to transmit the request entirely. */
		retcode = 1;
		/* fall through: we'll set a timeout, which will time out,
		 * and make us retransmit the request anyway. */
	default:
		/* all ok */
		log(EVDNS_LOG_DEBUG,
		    "Setting timeout for request %p, sent to nameserver %p", req, req->ns);
		if (evtimer_add(&req->timeout_event, &req->base->global_timeout) < 0) {
			log(EVDNS_LOG_WARN,
		      "Error from libevent when adding timer for request %p",
			    req);
			/* ???? Do more? */
		}
		req->tx_count++;
		req->transmit_me = 0;
		return retcode;
	}
}
