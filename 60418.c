evdns_requests_pump_waiting_queue(struct evdns_base *base) {
	ASSERT_LOCKED(base);
	while (base->global_requests_inflight < base->global_max_requests_inflight &&
		   base->global_requests_waiting) {
		struct request *req;

		EVUTIL_ASSERT(base->req_waiting_head);
		req = base->req_waiting_head;

		req->ns = nameserver_pick(base);
		if (!req->ns)
			return;

		/* move a request from the waiting queue to the inflight queue */
		req->ns->requests_inflight++;

		evdns_request_remove(req, &base->req_waiting_head);

		base->global_requests_waiting--;
		base->global_requests_inflight++;

		request_trans_id_set(req, transaction_id_pick(base));

		evdns_request_insert(req, &REQ_HEAD(base, req->trans_id));
		evdns_request_transmit(req);
		evdns_transmit(base);
	}
}
