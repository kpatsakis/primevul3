evdns_base_search_ndots_set(struct evdns_base *base, const int ndots) {
	EVDNS_LOCK(base);
	if (!base->global_search_state) base->global_search_state = search_state_new();
	if (base->global_search_state)
		base->global_search_state->ndots = ndots;
	EVDNS_UNLOCK(base);
}
