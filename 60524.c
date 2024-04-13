evutil_make_tcp_listen_socket_deferred(evutil_socket_t sock)
{
#if defined(EVENT__HAVE_NETINET_TCP_H) && defined(TCP_DEFER_ACCEPT)
	int one = 1;

	/* TCP_DEFER_ACCEPT tells the kernel to call defer accept() only after data
	 * has arrived and ready to read */ 
	return setsockopt(sock, IPPROTO_TCP, TCP_DEFER_ACCEPT, &one,
		(ev_socklen_t)sizeof(one)); 
#endif
	return 0;
}
