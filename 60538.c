evutil_socketpair(int family, int type, int protocol, evutil_socket_t fd[2])
{
#ifndef _WIN32
	return socketpair(family, type, protocol, fd);
#else
	return evutil_ersatz_socketpair_(family, type, protocol, fd);
#endif
}
