evdns_server_request_add_a_reply(struct evdns_server_request *req, const char *name, int n, const void *addrs, int ttl)
{
	return evdns_server_request_add_reply(
		  req, EVDNS_ANSWER_SECTION, name, TYPE_A, CLASS_INET,
		  ttl, n*4, 0, addrs);
}
