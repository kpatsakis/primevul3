server_port_free(struct evdns_server_port *port)
{
	EVUTIL_ASSERT(port);
	EVUTIL_ASSERT(!port->refcnt);
	EVUTIL_ASSERT(!port->pending_replies);
	if (port->socket > 0) {
		evutil_closesocket(port->socket);
		port->socket = -1;
	}
	(void) event_del(&port->event);
	event_debug_unassign(&port->event);
	EVTHREAD_FREE_LOCK(port->lock, EVTHREAD_LOCKTYPE_RECURSIVE);
	mm_free(port);
}
