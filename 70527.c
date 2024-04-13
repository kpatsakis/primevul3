static void compose_addr(struct sockaddr_in *sin, char *group, int port)
{
	memset(sin, 0, sizeof(*sin));
	sin->sin_family      = AF_INET;
	sin->sin_port        = htons(port);
	sin->sin_addr.s_addr = inet_addr(group);
}
