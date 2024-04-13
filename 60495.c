evutil_addrinfo_append_(struct evutil_addrinfo *first,
    struct evutil_addrinfo *append)
{
	struct evutil_addrinfo *ai = first;
	if (!ai)
		return append;
	while (ai->ai_next)
		ai = ai->ai_next;
	ai->ai_next = append;

	return first;
}
