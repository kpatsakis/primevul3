search_postfix_clear(struct evdns_base *base) {
	search_state_decref(base->global_search_state);

	base->global_search_state = search_state_new();
}
