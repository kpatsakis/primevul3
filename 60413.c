evdns_request_len(const size_t name_len) {
	return 96 + /* length of the DNS standard header */
		name_len + 2 +
		4;  /* space for the resource type */
}
