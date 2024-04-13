transaction_id_pick(struct evdns_base *base) {
	ASSERT_LOCKED(base);
	for (;;) {
		u16 trans_id;
		evutil_secure_rng_get_bytes(&trans_id, sizeof(trans_id));

		if (trans_id == 0xffff) continue;
		/* now check to see if that id is already inflight */
		if (request_find_from_trans_id(base, trans_id) == NULL)
			return trans_id;
	}
}
