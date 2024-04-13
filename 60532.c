evutil_sockaddr_is_loopback_(const struct sockaddr *addr)
{
	static const char LOOPBACK_S6[16] =
	    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1";
	if (addr->sa_family == AF_INET) {
		struct sockaddr_in *sin = (struct sockaddr_in *)addr;
		return (ntohl(sin->sin_addr.s_addr) & 0xff000000) == 0x7f000000;
	} else if (addr->sa_family == AF_INET6) {
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)addr;
		return !memcmp(sin6->sin6_addr.s6_addr, LOOPBACK_S6, 16);
	}
	return 0;
}
