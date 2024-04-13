evutil_fast_socket_closeonexec(evutil_socket_t fd)
{
#if !defined(_WIN32) && defined(EVENT__HAVE_SETFD)
	if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
		event_warn("fcntl(%d, F_SETFD)", fd);
		return -1;
	}
#endif
	return 0;
}
