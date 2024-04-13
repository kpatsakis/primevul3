add_cname_to_reply(struct evdns_getaddrinfo_request *data,
    struct evutil_addrinfo *ai)
{
	if (data->cname_result && ai) {
		ai->ai_canonname = data->cname_result;
		data->cname_result = NULL;
	}
}
