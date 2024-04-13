evdns_base_resolve_reverse(struct evdns_base *base, const struct in_addr *in, int flags, evdns_callback_type callback, void *ptr) {
	char buf[32];
	struct evdns_request *handle;
	struct request *req;
	u32 a;
	EVUTIL_ASSERT(in);
	a = ntohl(in->s_addr);
	evutil_snprintf(buf, sizeof(buf), "%d.%d.%d.%d.in-addr.arpa",
			(int)(u8)((a	)&0xff),
			(int)(u8)((a>>8 )&0xff),
			(int)(u8)((a>>16)&0xff),
			(int)(u8)((a>>24)&0xff));
	handle = mm_calloc(1, sizeof(*handle));
	if (handle == NULL)
		return NULL;
	log(EVDNS_LOG_DEBUG, "Resolve requested for %s (reverse)", buf);
	EVDNS_LOCK(base);
	req = request_new(base, handle, TYPE_PTR, buf, flags, callback, ptr);
	if (req)
		request_submit(req);
	if (handle->current_req == NULL) {
		mm_free(handle);
		handle = NULL;
	}
	EVDNS_UNLOCK(base);
	return (handle);
}
