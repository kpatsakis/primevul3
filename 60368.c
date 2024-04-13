evdns_base_clear_nameservers_and_suspend(struct evdns_base *base)
{
	struct nameserver *server, *started_at;
	int i;

	EVDNS_LOCK(base);
	server = base->server_head;
	started_at = base->server_head;
	if (!server) {
		EVDNS_UNLOCK(base);
		return 0;
	}
	while (1) {
		struct nameserver *next = server->next;
		(void) event_del(&server->event);
		if (evtimer_initialized(&server->timeout_event))
			(void) evtimer_del(&server->timeout_event);
		if (server->probe_request) {
			evdns_cancel_request(server->base, server->probe_request);
			server->probe_request = NULL;
		}
		if (server->socket >= 0)
			evutil_closesocket(server->socket);
		mm_free(server);
		if (next == started_at)
			break;
		server = next;
	}
	base->server_head = NULL;
	base->global_good_nameservers = 0;

	for (i = 0; i < base->n_req_heads; ++i) {
		struct request *req, *req_started_at;
		req = req_started_at = base->req_heads[i];
		while (req) {
			struct request *next = req->next;
			req->tx_count = req->reissue_count = 0;
			req->ns = NULL;
			/* ???? What to do about searches? */
			(void) evtimer_del(&req->timeout_event);
			req->trans_id = 0;
			req->transmit_me = 0;

			base->global_requests_waiting++;
			evdns_request_insert(req, &base->req_waiting_head);
			/* We want to insert these suspended elements at the front of
			 * the waiting queue, since they were pending before any of
			 * the waiting entries were added.  This is a circular list,
			 * so we can just shift the start back by one.*/
			base->req_waiting_head = base->req_waiting_head->prev;

			if (next == req_started_at)
				break;
			req = next;
		}
		base->req_heads[i] = NULL;
	}

	base->global_requests_inflight = 0;

	EVDNS_UNLOCK(base);
	return 0;
}
