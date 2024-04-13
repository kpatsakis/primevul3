evutil_socket_(int domain, int type, int protocol)
{
	evutil_socket_t r;
#if defined(SOCK_NONBLOCK) && defined(SOCK_CLOEXEC)
	r = socket(domain, type, protocol);
	if (r >= 0)
		return r;
	else if ((type & (SOCK_NONBLOCK|SOCK_CLOEXEC)) == 0)
		return -1;
#endif
#define SOCKET_TYPE_MASK (~(EVUTIL_SOCK_NONBLOCK|EVUTIL_SOCK_CLOEXEC))
	r = socket(domain, type & SOCKET_TYPE_MASK, protocol);
	if (r < 0)
		return -1;
	if (type & EVUTIL_SOCK_NONBLOCK) {
		if (evutil_fast_socket_nonblocking(r) < 0) {
			evutil_closesocket(r);
			return -1;
		}
	}
	if (type & EVUTIL_SOCK_CLOEXEC) {
		if (evutil_fast_socket_closeonexec(r) < 0) {
			evutil_closesocket(r);
			return -1;
		}
	}
	return r;
}
