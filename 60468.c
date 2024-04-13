search_request_new(struct evdns_base *base, struct evdns_request *handle,
		   int type, const char *const name, int flags,
		   evdns_callback_type user_callback, void *user_arg) {
	ASSERT_LOCKED(base);
	EVUTIL_ASSERT(type == TYPE_A || type == TYPE_AAAA);
	EVUTIL_ASSERT(handle->current_req == NULL);
	if ( ((flags & DNS_QUERY_NO_SEARCH) == 0) &&
	     base->global_search_state &&
		 base->global_search_state->num_domains) {
		/* we have some domains to search */
		struct request *req;
		if (string_num_dots(name) >= base->global_search_state->ndots) {
			req = request_new(base, handle, type, name, flags, user_callback, user_arg);
			if (!req) return NULL;
			handle->search_index = -1;
		} else {
			char *const new_name = search_make_new(base->global_search_state, 0, name);
			if (!new_name) return NULL;
			req = request_new(base, handle, type, new_name, flags, user_callback, user_arg);
			mm_free(new_name);
			if (!req) return NULL;
			handle->search_index = 0;
		}
		EVUTIL_ASSERT(handle->search_origname == NULL);
		handle->search_origname = mm_strdup(name);
		if (handle->search_origname == NULL) {
			/* XXX Should we dealloc req? If yes, how? */
			if (req)
				mm_free(req);
			return NULL;
		}
		handle->search_state = base->global_search_state;
		handle->search_flags = flags;
		base->global_search_state->refcount++;
		request_submit(req);
		return req;
	} else {
		struct request *const req = request_new(base, handle, type, name, flags, user_callback, user_arg);
		if (!req) return NULL;
		request_submit(req);
		return req;
	}
}
