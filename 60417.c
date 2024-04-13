evdns_request_transmit_to(struct request *req, struct nameserver *server) {
	int r;
	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);

	if (server->requests_inflight == 1 &&
		req->base->disable_when_inactive &&
		event_add(&server->event, NULL) < 0) {
		return 1;
	}

	r = sendto(server->socket, (void*)req->request, req->request_len, 0,
	    (struct sockaddr *)&server->address, server->addrlen);
	if (r < 0) {
		int err = evutil_socket_geterror(server->socket);
		if (EVUTIL_ERR_RW_RETRIABLE(err))
			return 1;
		nameserver_failed(req->ns, evutil_socket_error_to_string(err));
		return 2;
	} else if (r != (int)req->request_len) {
		return 1;  /* short write */
	} else {
		return 0;
	}
}
