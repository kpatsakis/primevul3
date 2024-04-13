static struct ifsock *find_iface(struct sockaddr *sa)
{
	struct ifsock *ifs;
	struct sockaddr_in *addr = (struct sockaddr_in *)sa;

	if (!sa)
		return NULL;

	LIST_FOREACH(ifs, &il, link) {
		if (ifs->addr.sin_addr.s_addr == addr->sin_addr.s_addr)
			return ifs;
	}

	return NULL;
}
