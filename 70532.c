static int filter_addr(struct sockaddr *sa)
{
	struct ifsock *ifs;
	struct sockaddr_in *sin = (struct sockaddr_in *)sa;

	if (!sa)
		return 1;

	if (sa->sa_family != AF_INET)
		return 1;

	if (sin->sin_addr.s_addr == htonl(INADDR_ANY))
		return 1;

	if (sin->sin_addr.s_addr == htonl(INADDR_LOOPBACK))
		return 1;

	ifs = find_outbound(sa);
	if (ifs) {
		if (ifs->addr.sin_addr.s_addr != htonl(INADDR_ANY))
			return 1;
	}

	return 0;
}
