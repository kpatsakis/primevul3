int register_socket(int in, int out, struct sockaddr *addr, struct sockaddr *mask, void (*cb)(int sd))
{
	struct ifsock *ifs;
	struct sockaddr_in *address = (struct sockaddr_in *)addr;
	struct sockaddr_in *netmask = (struct sockaddr_in *)mask;

	ifs = calloc(1, sizeof(*ifs));
	if (!ifs) {
		char *host = inet_ntoa(address->sin_addr);

		logit(LOG_ERR, "Failed registering host %s socket: %s", host, strerror(errno));
		return -1;
	}

	ifs->in   = in;
	ifs->out  = out;
	ifs->mod  = 1;
	ifs->cb   = cb;
	ifs->addr = *address;
	if (mask)
		ifs->mask = *netmask;
	LIST_INSERT_HEAD(&il, ifs, link);

	return 0;
}
