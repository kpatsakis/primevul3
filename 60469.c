search_reverse(struct evdns_base *base) {
	struct search_domain *cur, *prev = NULL, *next;
	ASSERT_LOCKED(base);
	cur = base->global_search_state->head;
	while (cur) {
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}

	base->global_search_state->head = prev;
}
