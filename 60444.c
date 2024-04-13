find_hosts_entry(struct evdns_base *base, const char *hostname,
    struct hosts_entry *find_after)
{
	struct hosts_entry *e;

	if (find_after)
		e = TAILQ_NEXT(find_after, next);
	else
		e = TAILQ_FIRST(&base->hostsdb);

	for (; e; e = TAILQ_NEXT(e, next)) {
		if (!evutil_ascii_strcasecmp(e->hostname, hostname))
			return e;
	}
	return NULL;
}
