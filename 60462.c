request_submit(struct request *const req) {
	struct evdns_base *base = req->base;
	ASSERT_LOCKED(base);
	ASSERT_VALID_REQUEST(req);
	if (req->ns) {
		/* if it has a nameserver assigned then this is going */
		/* straight into the inflight queue */
		evdns_request_insert(req, &REQ_HEAD(base, req->trans_id));

		base->global_requests_inflight++;
		req->ns->requests_inflight++;

		evdns_request_transmit(req);
	} else {
		evdns_request_insert(req, &base->req_waiting_head);
		base->global_requests_waiting++;
	}
}
