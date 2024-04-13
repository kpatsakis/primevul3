apply_numeric_port_hack(int port, struct evutil_addrinfo **ai)
{
	/* Now we run through the list and set the ports on all of the
	 * results where ports would make sense. */
	for ( ; *ai; ai = &(*ai)->ai_next) {
		struct sockaddr *sa = (*ai)->ai_addr;
		if (sa && sa->sa_family == AF_INET) {
			struct sockaddr_in *sin = (struct sockaddr_in*)sa;
			sin->sin_port = htons(port);
		} else if (sa && sa->sa_family == AF_INET6) {
			struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)sa;
			sin6->sin6_port = htons(port);
		} else {
			/* A numeric port makes no sense here; remove this one
			 * from the list. */
			struct evutil_addrinfo *victim = *ai;
			*ai = victim->ai_next;
			victim->ai_next = NULL;
			freeaddrinfo(victim);
		}
	}
}
