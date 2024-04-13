nameserver_prod_callback(evutil_socket_t fd, short events, void *arg) {
	struct nameserver *const ns = (struct nameserver *) arg;
	(void)fd;
	(void)events;

	EVDNS_LOCK(ns->base);
	nameserver_send_probe(ns);
	EVDNS_UNLOCK(ns->base);
}
