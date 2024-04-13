request_find_from_trans_id(struct evdns_base *base, u16 trans_id) {
	struct request *req = REQ_HEAD(base, trans_id);
	struct request *const started_at = req;

	ASSERT_LOCKED(base);

	if (req) {
		do {
			if (req->trans_id == trans_id) return req;
			req = req->next;
		} while (req != started_at);
	}

	return NULL;
}
