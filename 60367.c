evdns_add_server_port_with_base(struct event_base *base, evutil_socket_t socket, int flags, evdns_request_callback_fn_type cb, void *user_data)
{
	struct evdns_server_port *port;
	if (flags)
		return NULL; /* flags not yet implemented */
	if (!(port = mm_malloc(sizeof(struct evdns_server_port))))
		return NULL;
	memset(port, 0, sizeof(struct evdns_server_port));


	port->socket = socket;
	port->refcnt = 1;
	port->choked = 0;
	port->closing = 0;
	port->user_callback = cb;
	port->user_data = user_data;
	port->pending_replies = NULL;
	port->event_base = base;

	event_assign(&port->event, port->event_base,
				 port->socket, EV_READ | EV_PERSIST,
				 server_port_ready_callback, port);
	if (event_add(&port->event, NULL) < 0) {
		mm_free(port);
		return NULL;
	}
	EVTHREAD_ALLOC_LOCK(port->lock, EVTHREAD_LOCKTYPE_RECURSIVE);
	return port;
}
