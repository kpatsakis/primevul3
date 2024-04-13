evutil_freeaddrinfo(struct evutil_addrinfo *ai)
{
#ifdef EVENT__HAVE_GETADDRINFO
	if (!(ai->ai_flags & EVUTIL_AI_LIBEVENT_ALLOCATED)) {
		freeaddrinfo(ai);
		return;
	}
#endif
	while (ai) {
		struct evutil_addrinfo *next = ai->ai_next;
		if (ai->ai_canonname)
			mm_free(ai->ai_canonname);
		mm_free(ai);
		ai = next;
	}
}
