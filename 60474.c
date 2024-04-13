server_port_flush(struct evdns_server_port *port)
{
	struct server_request *req = port->pending_replies;
	ASSERT_LOCKED(port);
	while (req) {
		int r = sendto(port->socket, req->response, (int)req->response_len, 0,
			   (struct sockaddr*) &req->addr, (ev_socklen_t)req->addrlen);
		if (r < 0) {
			int err = evutil_socket_geterror(port->socket);
			if (EVUTIL_ERR_RW_RETRIABLE(err))
				return;
			log(EVDNS_LOG_WARN, "Error %s (%d) while writing response to port; dropping", evutil_socket_error_to_string(err), err);
		}
		if (server_request_free(req)) {
			/* we released the last reference to req->port. */
			return;
		} else {
			EVUTIL_ASSERT(req != port->pending_replies);
			req = port->pending_replies;
		}
	}

	/* We have no more pending requests; stop listening for 'writeable' events. */
	(void) event_del(&port->event);
	event_assign(&port->event, port->event_base,
				 port->socket, EV_READ | EV_PERSIST,
				 server_port_ready_callback, port);

	if (event_add(&port->event, NULL) < 0) {
		log(EVDNS_LOG_WARN, "Error from libevent when adding event for DNS server.");
		/* ???? Do more? */
	}
}
