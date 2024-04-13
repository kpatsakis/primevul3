evdns_getaddrinfo(struct evdns_base *dns_base,
    const char *nodename, const char *servname,
    const struct evutil_addrinfo *hints_in,
    evdns_getaddrinfo_cb cb, void *arg)
{
	struct evdns_getaddrinfo_request *data;
	struct evutil_addrinfo hints;
	struct evutil_addrinfo *res = NULL;
	int err;
	int port = 0;
	int want_cname = 0;

	if (!dns_base) {
		dns_base = current_base;
		if (!dns_base) {
			log(EVDNS_LOG_WARN,
			    "Call to getaddrinfo_async with no "
			    "evdns_base configured.");
			cb(EVUTIL_EAI_FAIL, NULL, arg); /* ??? better error? */
			return NULL;
		}
	}

	/* If we _must_ answer this immediately, do so. */
	if ((hints_in && (hints_in->ai_flags & EVUTIL_AI_NUMERICHOST))) {
		res = NULL;
		err = evutil_getaddrinfo(nodename, servname, hints_in, &res);
		cb(err, res, arg);
		return NULL;
	}

	if (hints_in) {
		memcpy(&hints, hints_in, sizeof(hints));
	} else {
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = PF_UNSPEC;
	}

	evutil_adjust_hints_for_addrconfig_(&hints);

	/* Now try to see if we _can_ answer immediately. */
	/* (It would be nice to do this by calling getaddrinfo directly, with
	 * AI_NUMERICHOST, on plaforms that have it, but we can't: there isn't
	 * a reliable way to distinguish the "that wasn't a numeric host!" case
	 * from any other EAI_NONAME cases.) */
	err = evutil_getaddrinfo_common_(nodename, servname, &hints, &res, &port);
	if (err != EVUTIL_EAI_NEED_RESOLVE) {
		cb(err, res, arg);
		return NULL;
	}

	/* If there is an entry in the hosts file, we should give it now. */
	if (!evdns_getaddrinfo_fromhosts(dns_base, nodename, &hints, port, &res)) {
		cb(0, res, arg);
		return NULL;
	}

	/* Okay, things are serious now. We're going to need to actually
	 * launch a request.
	 */
	data = mm_calloc(1,sizeof(struct evdns_getaddrinfo_request));
	if (!data) {
		cb(EVUTIL_EAI_MEMORY, NULL, arg);
		return NULL;
	}

	memcpy(&data->hints, &hints, sizeof(data->hints));
	data->port = (ev_uint16_t)port;
	data->ipv4_request.type = DNS_IPv4_A;
	data->ipv6_request.type = DNS_IPv6_AAAA;
	data->user_cb = cb;
	data->user_data = arg;
	data->evdns_base = dns_base;

	want_cname = (hints.ai_flags & EVUTIL_AI_CANONNAME);

	/* If we are asked for a PF_UNSPEC address, we launch two requests in
	 * parallel: one for an A address and one for an AAAA address.  We
	 * can't send just one request, since many servers only answer one
	 * question per DNS request.
	 *
	 * Once we have the answer to one request, we allow for a short
	 * timeout before we report it, to see if the other one arrives.  If
	 * they both show up in time, then we report both the answers.
	 *
	 * If too many addresses of one type time out or fail, we should stop
	 * launching those requests. (XXX we don't do that yet.)
	 */

	if (hints.ai_family != PF_INET6) {
		log(EVDNS_LOG_DEBUG, "Sending request for %s on ipv4 as %p",
		    nodename, &data->ipv4_request);

		data->ipv4_request.r = evdns_base_resolve_ipv4(dns_base,
		    nodename, 0, evdns_getaddrinfo_gotresolve,
		    &data->ipv4_request);
		if (want_cname && data->ipv4_request.r)
			data->ipv4_request.r->current_req->put_cname_in_ptr =
			    &data->cname_result;
	}
	if (hints.ai_family != PF_INET) {
		log(EVDNS_LOG_DEBUG, "Sending request for %s on ipv6 as %p",
		    nodename, &data->ipv6_request);

		data->ipv6_request.r = evdns_base_resolve_ipv6(dns_base,
		    nodename, 0, evdns_getaddrinfo_gotresolve,
		    &data->ipv6_request);
		if (want_cname && data->ipv6_request.r)
			data->ipv6_request.r->current_req->put_cname_in_ptr =
			    &data->cname_result;
	}

	evtimer_assign(&data->timeout, dns_base->event_base,
	    evdns_getaddrinfo_timeout_cb, data);

	if (data->ipv4_request.r || data->ipv6_request.r) {
		return data;
	} else {
		mm_free(data);
		cb(EVUTIL_EAI_FAIL, NULL, arg);
		return NULL;
	}
}
