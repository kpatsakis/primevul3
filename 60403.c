evdns_getaddrinfo_timeout_cb(evutil_socket_t fd, short what, void *ptr)
{
	int v4_timedout = 0, v6_timedout = 0;
	struct evdns_getaddrinfo_request *data = ptr;

	/* Cancel any pending requests, and note which one */
	if (data->ipv4_request.r) {
		/* XXXX This does nothing if the request's callback is already
		 * running (pending_cb is set). */
		evdns_cancel_request(NULL, data->ipv4_request.r);
		v4_timedout = 1;
		EVDNS_LOCK(data->evdns_base);
		++data->evdns_base->getaddrinfo_ipv4_timeouts;
		EVDNS_UNLOCK(data->evdns_base);
	}
	if (data->ipv6_request.r) {
		/* XXXX This does nothing if the request's callback is already
		 * running (pending_cb is set). */
		evdns_cancel_request(NULL, data->ipv6_request.r);
		v6_timedout = 1;
		EVDNS_LOCK(data->evdns_base);
		++data->evdns_base->getaddrinfo_ipv6_timeouts;
		EVDNS_UNLOCK(data->evdns_base);
	}

	/* We only use this timeout callback when we have an answer for
	 * one address. */
	EVUTIL_ASSERT(!v4_timedout || !v6_timedout);

	/* Report the outcome of the other request that didn't time out. */
	if (data->pending_result) {
		add_cname_to_reply(data, data->pending_result);
		data->user_cb(0, data->pending_result, data->user_data);
		data->pending_result = NULL;
	} else {
		int e = data->pending_error;
		if (!e)
			e = EVUTIL_EAI_AGAIN;
		data->user_cb(e, NULL, data->user_data);
	}

	data->user_cb = NULL; /* prevent double-call if evdns callbacks are
			       * in-progress. XXXX It would be better if this
			       * weren't necessary. */

	if (!v4_timedout && !v6_timedout) {
		/* should be impossible? XXXX */
		free_getaddrinfo_request(data);
	}
}
