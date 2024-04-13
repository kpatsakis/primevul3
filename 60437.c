evdns_server_request_respond(struct evdns_server_request *req_, int err)
{
	struct server_request *req = TO_SERVER_REQUEST(req_);
	struct evdns_server_port *port = req->port;
	int r = -1;

	EVDNS_LOCK(port);
	if (!req->response) {
		if ((r = evdns_server_request_format_response(req, err))<0)
			goto done;
	}

	r = sendto(port->socket, req->response, (int)req->response_len, 0,
			   (struct sockaddr*) &req->addr, (ev_socklen_t)req->addrlen);
	if (r<0) {
		int sock_err = evutil_socket_geterror(port->socket);
		if (EVUTIL_ERR_RW_RETRIABLE(sock_err))
			goto done;

		if (port->pending_replies) {
			req->prev_pending = port->pending_replies->prev_pending;
			req->next_pending = port->pending_replies;
			req->prev_pending->next_pending =
				req->next_pending->prev_pending = req;
		} else {
			req->prev_pending = req->next_pending = req;
			port->pending_replies = req;
			port->choked = 1;

			(void) event_del(&port->event);
			event_assign(&port->event, port->event_base, port->socket, (port->closing?0:EV_READ) | EV_WRITE | EV_PERSIST, server_port_ready_callback, port);

			if (event_add(&port->event, NULL) < 0) {
				log(EVDNS_LOG_WARN, "Error from libevent when adding event for DNS server");
			}

		}

		r = 1;
		goto done;
	}
	if (server_request_free(req)) {
		r = 0;
		goto done;
	}

	if (port->pending_replies)
		server_port_flush(port);

	r = 0;
done:
	EVDNS_UNLOCK(port);
	return r;
}
