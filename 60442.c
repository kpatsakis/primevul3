evdns_shutdown(int fail_requests)
{
	if (current_base) {
		struct evdns_base *b = current_base;
		current_base = NULL;
		evdns_base_free(b, fail_requests);
	}
	evdns_log_fn = NULL;
}
