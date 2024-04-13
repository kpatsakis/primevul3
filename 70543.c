static void send_message(struct ifsock *ifs, char *type, struct sockaddr *sa)
{
	int s;
	size_t i, len, note = 0;
	ssize_t num;
	char host[NI_MAXHOST];
	char buf[MAX_PKT_SIZE];
	struct sockaddr dest;
	struct sockaddr_in *sin = (struct sockaddr_in *)sa;

	gethostname(hostname, sizeof(hostname));
	s = getnameinfo((struct sockaddr *)&ifs->addr, sizeof(struct sockaddr_in), host, sizeof(host), NULL, 0, NI_NUMERICHOST);
	if (s) {
		logit(LOG_WARNING, "Failed getnameinfo(): %s", gai_strerror(s));
		return;
	}

	if (ifs->addr.sin_addr.s_addr == htonl(INADDR_ANY))
		return;
	if (ifs->out == -1)
		return;

	if (!strcmp(type, SSDP_ST_ALL))
		type = NULL;

	memset(buf, 0, sizeof(buf));
	if (sin)
		compose_response(type, host, buf, sizeof(buf));
	else
		compose_notify(type, host, buf, sizeof(buf));

	if (!sin) {
		note = 1;
		compose_addr((struct sockaddr_in *)&dest, MC_SSDP_GROUP, MC_SSDP_PORT);
		sin = (struct sockaddr_in *)&dest;
	}

	logit(LOG_DEBUG, "Sending %s from %s ...", !note ? "reply" : "notify", host);
	num = sendto(ifs->out, buf, strlen(buf), 0, sin, sizeof(struct sockaddr_in));
	if (num < 0)
		logit(LOG_WARNING, "Failed sending SSDP %s, type: %s: %s", !note ? "reply" : "notify", type, strerror(errno));
}
