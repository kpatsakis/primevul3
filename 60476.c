server_port_read(struct evdns_server_port *s) {
	u8 packet[1500];
	struct sockaddr_storage addr;
	ev_socklen_t addrlen;
	int r;
	ASSERT_LOCKED(s);

	for (;;) {
		addrlen = sizeof(struct sockaddr_storage);
		r = recvfrom(s->socket, (void*)packet, sizeof(packet), 0,
					 (struct sockaddr*) &addr, &addrlen);
		if (r < 0) {
			int err = evutil_socket_geterror(s->socket);
			if (EVUTIL_ERR_RW_RETRIABLE(err))
				return;
			log(EVDNS_LOG_WARN,
			    "Error %s (%d) while reading request.",
			    evutil_socket_error_to_string(err), err);
			return;
		}
		request_parse(packet, r, s, (struct sockaddr*) &addr, addrlen);
	}
}
