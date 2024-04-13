evdns_close_server_port(struct evdns_server_port *port)
{
	EVDNS_LOCK(port);
	if (--port->refcnt == 0) {
		EVDNS_UNLOCK(port);
		server_port_free(port);
	} else {
		port->closing = 1;
	}
}
