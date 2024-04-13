evdns_add_server_port(evutil_socket_t socket, int flags, evdns_request_callback_fn_type cb, void *user_data)
{
	return evdns_add_server_port_with_base(NULL, socket, flags, cb, user_data);
}
