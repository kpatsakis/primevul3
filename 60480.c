sockaddr_getport(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return ntohs(((struct sockaddr_in *)sa)->sin_port);
	} else if (sa->sa_family == AF_INET6) {
		return ntohs(((struct sockaddr_in6 *)sa)->sin6_port);
	} else {
		return 0;
	}
}
