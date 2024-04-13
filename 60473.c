search_try_next(struct evdns_request *const handle) {
	struct request *req = handle->current_req;
	struct evdns_base *base = req->base;
	struct request *newreq;
	ASSERT_LOCKED(base);
	if (handle->search_state) {
		/* it is part of a search */
		char *new_name;
		handle->search_index++;
		if (handle->search_index >= handle->search_state->num_domains) {
			/* no more postfixes to try, however we may need to try */
			/* this name without a postfix */
			if (string_num_dots(handle->search_origname) < handle->search_state->ndots) {
				/* yep, we need to try it raw */
				newreq = request_new(base, NULL, req->request_type, handle->search_origname, handle->search_flags, req->user_callback, req->user_pointer);
				log(EVDNS_LOG_DEBUG, "Search: trying raw query %s", handle->search_origname);
				if (newreq) {
					search_request_finished(handle);
					goto submit_next;
				}
			}
			return 1;
		}

		new_name = search_make_new(handle->search_state, handle->search_index, handle->search_origname);
		if (!new_name) return 1;
		log(EVDNS_LOG_DEBUG, "Search: now trying %s (%d)", new_name, handle->search_index);
		newreq = request_new(base, NULL, req->request_type, new_name, handle->search_flags, req->user_callback, req->user_pointer);
		mm_free(new_name);
		if (!newreq) return 1;
		goto submit_next;
	}
	return 1;

submit_next:
	request_finished(req, &REQ_HEAD(req->base, req->trans_id), 0);
	handle->current_req = newreq;
	newreq->handle = handle;
	request_submit(newreq);
	return 0;
}
