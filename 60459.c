request_new(struct evdns_base *base, struct evdns_request *handle, int type,
	    const char *name, int flags, evdns_callback_type callback,
	    void *user_ptr) {

	const char issuing_now =
	    (base->global_requests_inflight < base->global_max_requests_inflight) ? 1 : 0;

	const size_t name_len = strlen(name);
	const size_t request_max_len = evdns_request_len(name_len);
	const u16 trans_id = issuing_now ? transaction_id_pick(base) : 0xffff;
	/* the request data is alloced in a single block with the header */
	struct request *const req =
	    mm_malloc(sizeof(struct request) + request_max_len);
	int rlen;
	char namebuf[256];
	(void) flags;

	ASSERT_LOCKED(base);

	if (!req) return NULL;

	if (name_len >= sizeof(namebuf)) {
		mm_free(req);
		return NULL;
	}

	memset(req, 0, sizeof(struct request));
	req->base = base;

	evtimer_assign(&req->timeout_event, req->base->event_base, evdns_request_timeout_callback, req);

	if (base->global_randomize_case) {
		unsigned i;
		char randbits[(sizeof(namebuf)+7)/8];
		strlcpy(namebuf, name, sizeof(namebuf));
		evutil_secure_rng_get_bytes(randbits, (name_len+7)/8);
		for (i = 0; i < name_len; ++i) {
			if (EVUTIL_ISALPHA_(namebuf[i])) {
				if ((randbits[i >> 3] & (1<<(i & 7))))
					namebuf[i] |= 0x20;
				else
					namebuf[i] &= ~0x20;
			}
		}
		name = namebuf;
	}

	/* request data lives just after the header */
	req->request = ((u8 *) req) + sizeof(struct request);
	/* denotes that the request data shouldn't be free()ed */
	req->request_appended = 1;
	rlen = evdns_request_data_build(name, name_len, trans_id,
	    type, CLASS_INET, req->request, request_max_len);
	if (rlen < 0)
		goto err1;

	req->request_len = rlen;
	req->trans_id = trans_id;
	req->tx_count = 0;
	req->request_type = type;
	req->user_pointer = user_ptr;
	req->user_callback = callback;
	req->ns = issuing_now ? nameserver_pick(base) : NULL;
	req->next = req->prev = NULL;
	req->handle = handle;
	if (handle) {
		handle->current_req = req;
		handle->base = base;
	}

	return req;
err1:
	mm_free(req);
	return NULL;
}
