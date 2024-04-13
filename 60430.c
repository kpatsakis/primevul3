evdns_server_request_add_aaaa_reply(struct evdns_server_request *req, const char *name, int n, const void *addrs, int ttl)
{
	return evdns_server_request_add_reply(
		  req, EVDNS_ANSWER_SECTION, name, TYPE_AAAA, CLASS_INET,
		  ttl, n*16, 0, addrs);
}
