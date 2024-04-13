nameserver_ready_callback(evutil_socket_t fd, short events, void *arg) {
	struct nameserver *ns = (struct nameserver *) arg;
	(void)fd;

	EVDNS_LOCK(ns->base);
	if (events & EV_WRITE) {
		ns->choked = 0;
		if (!evdns_transmit(ns->base)) {
			nameserver_write_waiting(ns, 0);
		}
	}
	if (events & EV_READ) {
		nameserver_read(ns);
	}
	EVDNS_UNLOCK(ns->base);
}
