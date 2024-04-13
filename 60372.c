evdns_base_get_nameserver_addr(struct evdns_base *base, int idx,
    struct sockaddr *sa, ev_socklen_t len)
{
	int result = -1;
	int i;
	struct nameserver *server;
	EVDNS_LOCK(base);
	server = base->server_head;
	for (i = 0; i < idx && server; ++i, server = server->next) {
		if (server->next == base->server_head)
			goto done;
	}
	if (! server)
		goto done;

	if (server->addrlen > len) {
		result = (int) server->addrlen;
		goto done;
	}

	memcpy(sa, &server->address, server->addrlen);
	result = (int) server->addrlen;
done:
	EVDNS_UNLOCK(base);
	return result;
}
