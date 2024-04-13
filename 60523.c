evutil_make_socket_nonblocking(evutil_socket_t fd)
{
#ifdef _WIN32
	{
		unsigned long nonblocking = 1;
		if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
			event_sock_warn(fd, "fcntl(%d, F_GETFL)", (int)fd);
			return -1;
		}
	}
#else
	{
		int flags;
		if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
			event_warn("fcntl(%d, F_GETFL)", fd);
			return -1;
		}
		if (!(flags & O_NONBLOCK)) {
			if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
				event_warn("fcntl(%d, F_SETFL)", fd);
				return -1;
			}
		}
	}
#endif
	return 0;
}
