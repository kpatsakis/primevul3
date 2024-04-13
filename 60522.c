evutil_make_socket_closeonexec(evutil_socket_t fd)
{
#if !defined(_WIN32) && defined(EVENT__HAVE_SETFD)
	int flags;
	if ((flags = fcntl(fd, F_GETFD, NULL)) < 0) {
		event_warn("fcntl(%d, F_GETFD)", fd);
		return -1;
	}
	if (!(flags & FD_CLOEXEC)) {
		if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1) {
			event_warn("fcntl(%d, F_SETFD)", fd);
			return -1;
		}
	}
#endif
	return 0;
}
