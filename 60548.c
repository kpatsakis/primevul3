evdns_base_free_and_unlock(struct evdns_base *base, int fail_requests)
{
	struct nameserver *server, *server_next;
	struct search_domain *dom, *dom_next;
	int i;

	/* Requires that we hold the lock. */

	/* TODO(nickm) we might need to refcount here. */

	for (i = 0; i < base->n_req_heads; ++i) {
		while (base->req_heads[i]) {
			if (fail_requests)
				reply_schedule_callback(base->req_heads[i], 0, DNS_ERR_SHUTDOWN, NULL);
			request_finished(base->req_heads[i], &REQ_HEAD(base, base->req_heads[i]->trans_id), 1);
		}
	}
	while (base->req_waiting_head) {
		if (fail_requests)
			reply_schedule_callback(base->req_waiting_head, 0, DNS_ERR_SHUTDOWN, NULL);
		request_finished(base->req_waiting_head, &base->req_waiting_head, 1);
	}
	base->global_requests_inflight = base->global_requests_waiting = 0;

	for (server = base->server_head; server; server = server_next) {
		server_next = server->next;
		evdns_nameserver_free(server);
		if (server_next == base->server_head)
			break;
	}
	base->server_head = NULL;
	base->global_good_nameservers = 0;

	if (base->global_search_state) {
		for (dom = base->global_search_state->head; dom; dom = dom_next) {
			dom_next = dom->next;
			mm_free(dom);
		}
		mm_free(base->global_search_state);
		base->global_search_state = NULL;
	}

	{
		struct hosts_entry *victim;
		while ((victim = TAILQ_FIRST(&base->hostsdb))) {
			TAILQ_REMOVE(&base->hostsdb, victim, next);
			mm_free(victim);
		}
	}

	mm_free(base->req_heads);

	EVDNS_UNLOCK(base);
	EVTHREAD_FREE_LOCK(base->lock, EVTHREAD_LOCKTYPE_RECURSIVE);

	mm_free(base);
}
