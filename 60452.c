nameserver_send_probe(struct nameserver *const ns) {
	struct evdns_request *handle;
	struct request *req;
	char addrbuf[128];
	/* here we need to send a probe to a given nameserver */
	/* in the hope that it is up now. */

	ASSERT_LOCKED(ns->base);
	log(EVDNS_LOG_DEBUG, "Sending probe to %s",
	    evutil_format_sockaddr_port_(
		    (struct sockaddr *)&ns->address,
		    addrbuf, sizeof(addrbuf)));
	handle = mm_calloc(1, sizeof(*handle));
	if (!handle) return;
	req = request_new(ns->base, handle, TYPE_A, "google.com", DNS_QUERY_NO_SEARCH, nameserver_probe_callback, ns);
	if (!req) {
		mm_free(handle);
		return;
	}
	ns->probe_request = handle;
	/* we force this into the inflight queue no matter what */
	request_trans_id_set(req, transaction_id_pick(ns->base));
	req->ns = ns;
	request_submit(req);
}
