request_reissue(struct request *req) {
	const struct nameserver *const last_ns = req->ns;
	ASSERT_LOCKED(req->base);
	ASSERT_VALID_REQUEST(req);
	/* the last nameserver should have been marked as failing */
	/* by the caller of this function, therefore pick will try */
	/* not to return it */
	request_swap_ns(req, nameserver_pick(req->base));
	if (req->ns == last_ns) {
		/* ... but pick did return it */
		/* not a lot of point in trying again with the */
		/* same server */
		return 1;
	}

	req->reissue_count++;
	req->tx_count = 0;
	req->transmit_me = 1;

	return 0;
}
