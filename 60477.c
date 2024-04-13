server_port_ready_callback(evutil_socket_t fd, short events, void *arg) {
	struct evdns_server_port *port = (struct evdns_server_port *) arg;
	(void) fd;

	EVDNS_LOCK(port);
	if (events & EV_WRITE) {
		port->choked = 0;
		server_port_flush(port);
	}
	if (events & EV_READ) {
		server_port_read(port);
	}
	EVDNS_UNLOCK(port);
}
