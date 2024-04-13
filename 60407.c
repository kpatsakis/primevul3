evdns_nameserver_add_impl_(struct evdns_base *base, const struct sockaddr *address, int addrlen) {
	/* first check to see if we already have this nameserver */

	const struct nameserver *server = base->server_head, *const started_at = base->server_head;
	struct nameserver *ns;
	int err = 0;
	char addrbuf[128];

	ASSERT_LOCKED(base);
	if (server) {
		do {
			if (!evutil_sockaddr_cmp((struct sockaddr*)&server->address, address, 1)) return 3;
			server = server->next;
		} while (server != started_at);
	}
	if (addrlen > (int)sizeof(ns->address)) {
		log(EVDNS_LOG_DEBUG, "Addrlen %d too long.", (int)addrlen);
		return 2;
	}

	ns = (struct nameserver *) mm_malloc(sizeof(struct nameserver));
	if (!ns) return -1;

	memset(ns, 0, sizeof(struct nameserver));
	ns->base = base;

	evtimer_assign(&ns->timeout_event, ns->base->event_base, nameserver_prod_callback, ns);

	ns->socket = evutil_socket_(address->sa_family,
	    SOCK_DGRAM|EVUTIL_SOCK_NONBLOCK|EVUTIL_SOCK_CLOEXEC, 0);
	if (ns->socket < 0) { err = 1; goto out1; }

	if (base->global_outgoing_addrlen &&
	    !evutil_sockaddr_is_loopback_(address)) {
		if (bind(ns->socket,
			(struct sockaddr*)&base->global_outgoing_address,
			base->global_outgoing_addrlen) < 0) {
			log(EVDNS_LOG_WARN,"Couldn't bind to outgoing address");
			err = 2;
			goto out2;
		}
	}

	memcpy(&ns->address, address, addrlen);
	ns->addrlen = addrlen;
	ns->state = 1;
	event_assign(&ns->event, ns->base->event_base, ns->socket,
				 EV_READ | EV_PERSIST, nameserver_ready_callback, ns);
	if (!base->disable_when_inactive && event_add(&ns->event, NULL) < 0) {
		err = 2;
		goto out2;
	}

	log(EVDNS_LOG_DEBUG, "Added nameserver %s as %p",
	    evutil_format_sockaddr_port_(address, addrbuf, sizeof(addrbuf)), ns);

	/* insert this nameserver into the list of them */
	if (!base->server_head) {
		ns->next = ns->prev = ns;
		base->server_head = ns;
	} else {
		ns->next = base->server_head->next;
		ns->prev = base->server_head;
		base->server_head->next = ns;
		ns->next->prev = ns;
	}

	base->global_good_nameservers++;

	return 0;

out2:
	evutil_closesocket(ns->socket);
out1:
	event_debug_unassign(&ns->event);
	mm_free(ns);
	log(EVDNS_LOG_WARN, "Unable to add nameserver %s: error %d",
	    evutil_format_sockaddr_port_(address, addrbuf, sizeof(addrbuf)), err);
	return err;
}
