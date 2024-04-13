evdns_getaddrinfo_fromhosts(struct evdns_base *base,
    const char *nodename, struct evutil_addrinfo *hints, ev_uint16_t port,
    struct evutil_addrinfo **res)
{
	int n_found = 0;
	struct hosts_entry *e;
	struct evutil_addrinfo *ai=NULL;
	int f = hints->ai_family;

	EVDNS_LOCK(base);
	for (e = find_hosts_entry(base, nodename, NULL); e;
	    e = find_hosts_entry(base, nodename, e)) {
		struct evutil_addrinfo *ai_new;
		++n_found;
		if ((e->addr.sa.sa_family == AF_INET && f == PF_INET6) ||
		    (e->addr.sa.sa_family == AF_INET6 && f == PF_INET))
			continue;
		ai_new = evutil_new_addrinfo_(&e->addr.sa, e->addrlen, hints);
		if (!ai_new) {
			n_found = 0;
			goto out;
		}
		sockaddr_setport(ai_new->ai_addr, port);
		ai = evutil_addrinfo_append_(ai, ai_new);
	}
	EVDNS_UNLOCK(base);
out:
	if (n_found) {
		/* Note that we return an empty answer if we found entries for
		 * this hostname but none were of the right address type. */
		*res = ai;
		return 0;
	} else {
		if (ai)
			evutil_freeaddrinfo(ai);
		return -1;
	}
}
