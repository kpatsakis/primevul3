server_request_free(struct server_request *req)
{
	int i, rc=1, lock=0;
	if (req->base.questions) {
		for (i = 0; i < req->base.nquestions; ++i)
			mm_free(req->base.questions[i]);
		mm_free(req->base.questions);
	}

	if (req->port) {
		EVDNS_LOCK(req->port);
		lock=1;
		if (req->port->pending_replies == req) {
			if (req->next_pending && req->next_pending != req)
				req->port->pending_replies = req->next_pending;
			else
				req->port->pending_replies = NULL;
		}
		rc = --req->port->refcnt;
	}

	if (req->response) {
		mm_free(req->response);
	}

	server_request_free_answers(req);

	if (req->next_pending && req->next_pending != req) {
		req->next_pending->prev_pending = req->prev_pending;
		req->prev_pending->next_pending = req->next_pending;
	}

	if (rc == 0) {
		EVDNS_UNLOCK(req->port); /* ????? nickm */
		server_port_free(req->port);
		mm_free(req);
		return (1);
	}
	if (lock)
		EVDNS_UNLOCK(req->port);
	mm_free(req);
	return (0);
}
