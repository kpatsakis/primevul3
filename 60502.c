evutil_fast_socket_nonblocking(evutil_socket_t fd)
{
#ifdef _WIN32
	return evutil_make_socket_nonblocking(fd);
#else
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
		event_warn("fcntl(%d, F_SETFL)", fd);
		return -1;
	}
	return 0;
#endif
}
