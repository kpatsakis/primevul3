nameserver_read(struct nameserver *ns) {
	struct sockaddr_storage ss;
	ev_socklen_t addrlen = sizeof(ss);
	u8 packet[1500];
	char addrbuf[128];
	ASSERT_LOCKED(ns->base);

	for (;;) {
		const int r = recvfrom(ns->socket, (void*)packet,
		    sizeof(packet), 0,
		    (struct sockaddr*)&ss, &addrlen);
		if (r < 0) {
			int err = evutil_socket_geterror(ns->socket);
			if (EVUTIL_ERR_RW_RETRIABLE(err))
				return;
			nameserver_failed(ns,
			    evutil_socket_error_to_string(err));
			return;
		}
		if (evutil_sockaddr_cmp((struct sockaddr*)&ss,
			(struct sockaddr*)&ns->address, 0)) {
			log(EVDNS_LOG_WARN, "Address mismatch on received "
			    "DNS packet.  Apparent source was %s",
			    evutil_format_sockaddr_port_(
				    (struct sockaddr *)&ss,
				    addrbuf, sizeof(addrbuf)));
			return;
		}

		ns->timedout = 0;
		reply_parse(ns->base, packet, r);
	}
}
