evdns_server_request_add_cname_reply(struct evdns_server_request *req, const char *name, const char *cname, int ttl)
{
	return evdns_server_request_add_reply(
		  req, EVDNS_ANSWER_SECTION, name, TYPE_CNAME, CLASS_INET,
		  ttl, -1, 1, cname);
}
