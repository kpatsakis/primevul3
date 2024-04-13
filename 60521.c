evutil_make_listen_socket_reuseable_port(evutil_socket_t sock)
{
#if defined __linux__ && defined(SO_REUSEPORT)
	int one = 1;
	/* REUSEPORT on Linux 3.9+ means, "Multiple servers (processes or
	 * threads) can bind to the same port if they each set the option. */
	return setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (void*) &one,
	    (ev_socklen_t)sizeof(one));
#else
	return 0;
#endif
}
