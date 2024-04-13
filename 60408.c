evdns_nameserver_free(struct nameserver *server)
{
	if (server->socket >= 0)
		evutil_closesocket(server->socket);
	(void) event_del(&server->event);
	event_debug_unassign(&server->event);
	if (server->state == 0)
		(void) event_del(&server->timeout_event);
	if (server->probe_request) {
		evdns_cancel_request(server->base, server->probe_request);
		server->probe_request = NULL;
	}
	event_debug_unassign(&server->timeout_event);
	mm_free(server);
}
