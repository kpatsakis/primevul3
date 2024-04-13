static struct ifsock *find_outbound(struct sockaddr *sa)
{
	in_addr_t cand;
	struct ifsock *ifs;
	struct sockaddr_in *addr = (struct sockaddr_in *)sa;

	cand = addr->sin_addr.s_addr;
	LIST_FOREACH(ifs, &il, link) {
		in_addr_t a, m;

		a = ifs->addr.sin_addr.s_addr;
		m = ifs->mask.sin_addr.s_addr;
		if (a == htonl(INADDR_ANY) || m == htonl(INADDR_ANY))
			continue;

		if ((a & m) == (cand & m))
			return ifs;
	}

	return NULL;
}
