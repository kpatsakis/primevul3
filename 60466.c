search_postfix_add(struct evdns_base *base, const char *domain) {
	size_t domain_len;
	struct search_domain *sdomain;
	while (domain[0] == '.') domain++;
	domain_len = strlen(domain);

	ASSERT_LOCKED(base);
	if (!base->global_search_state) base->global_search_state = search_state_new();
	if (!base->global_search_state) return;
	base->global_search_state->num_domains++;

	sdomain = (struct search_domain *) mm_malloc(sizeof(struct search_domain) + domain_len);
	if (!sdomain) return;
	memcpy( ((u8 *) sdomain) + sizeof(struct search_domain), domain, domain_len);
	sdomain->next = base->global_search_state->head;
	sdomain->len = (int) domain_len;

	base->global_search_state->head = sdomain;
}
