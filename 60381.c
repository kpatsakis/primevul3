evdns_base_resolve_ipv6(struct evdns_base *base,
    const char *name, int flags,
    evdns_callback_type callback, void *ptr)
{
	struct evdns_request *handle;
	struct request *req;
	log(EVDNS_LOG_DEBUG, "Resolve requested for %s", name);
	handle = mm_calloc(1, sizeof(*handle));
	if (handle == NULL)
		return NULL;
	EVDNS_LOCK(base);
	if (flags & DNS_QUERY_NO_SEARCH) {
		req = request_new(base, handle, TYPE_AAAA, name, flags,
				  callback, ptr);
		if (req)
			request_submit(req);
	} else {
		search_request_new(base, handle, TYPE_AAAA, name, flags,
		    callback, ptr);
	}
	if (handle->current_req == NULL) {
		mm_free(handle);
		handle = NULL;
	}
	EVDNS_UNLOCK(base);
	return handle;
}
