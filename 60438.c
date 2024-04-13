evdns_server_request_set_flags(struct evdns_server_request *exreq, int flags)
{
	struct server_request *req = TO_SERVER_REQUEST(exreq);
	req->base.flags &= ~(EVDNS_FLAGS_AA|EVDNS_FLAGS_RD);
	req->base.flags |= flags;
}
