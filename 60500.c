evutil_eventfd_(unsigned initval, int flags)
{
#if defined(EVENT__HAVE_EVENTFD) && defined(EVENT__HAVE_SYS_EVENTFD_H)
	int r;
#if defined(EFD_CLOEXEC) && defined(EFD_NONBLOCK)
	r = eventfd(initval, flags);
	if (r >= 0 || flags == 0)
		return r;
#endif
	r = eventfd(initval, 0);
	if (r < 0)
		return r;
	if (flags & EVUTIL_EFD_CLOEXEC) {
		if (evutil_fast_socket_closeonexec(r) < 0) {
			evutil_closesocket(r);
			return -1;
		}
	}
	if (flags & EVUTIL_EFD_NONBLOCK) {
		if (evutil_fast_socket_nonblocking(r) < 0) {
			evutil_closesocket(r);
			return -1;
		}
	}
	return r;
#else
	return -1;
#endif
}
