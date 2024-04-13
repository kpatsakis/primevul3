sockaddr_setport(struct sockaddr *sa, ev_uint16_t port)
{
	if (sa->sa_family == AF_INET) {
		((struct sockaddr_in *)sa)->sin_port = htons(port);
	} else if (sa->sa_family == AF_INET6) {
		((struct sockaddr_in6 *)sa)->sin6_port = htons(port);
	}
}
