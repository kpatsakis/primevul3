static int multicast_init(void)
{
	int sd;
	struct sockaddr sa;
	struct sockaddr_in *sin = (struct sockaddr_in *)&sa;

	sd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	if (sd < 0) {
		logit(LOG_ERR, "Failed opening multicast socket: %s", strerror(errno));
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = inet_addr(MC_SSDP_GROUP);
	sin->sin_port = htons(MC_SSDP_PORT);

	if (bind(sd, &sa, sizeof(*sin)) < 0) {
		close(sd);
		logit(LOG_ERR, "Failed binding to %s:%d: %s", inet_ntoa(sin->sin_addr), MC_SSDP_PORT, strerror(errno));
		return -1;
	}

	register_socket(sd, -1, &sa, NULL, ssdp_recv);

	return sd;
}
