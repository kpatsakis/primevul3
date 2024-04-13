evdns_server_request_drop(struct evdns_server_request *req_)
{
	struct server_request *req = TO_SERVER_REQUEST(req_);
	server_request_free(req);
	return 0;
}
