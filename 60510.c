evutil_getaddrinfo_infer_protocols(struct evutil_addrinfo *hints)
{
	/* If we can guess the protocol from the socktype, do so. */
	if (!hints->ai_protocol && hints->ai_socktype) {
		if (hints->ai_socktype == SOCK_DGRAM)
			hints->ai_protocol = IPPROTO_UDP;
		else if (hints->ai_socktype == SOCK_STREAM)
			hints->ai_protocol = IPPROTO_TCP;
	}

	/* Set the socktype if it isn't set. */
	if (!hints->ai_socktype && hints->ai_protocol) {
		if (hints->ai_protocol == IPPROTO_UDP)
			hints->ai_socktype = SOCK_DGRAM;
		else if (hints->ai_protocol == IPPROTO_TCP)
			hints->ai_socktype = SOCK_STREAM;
#ifdef IPPROTO_SCTP
		else if (hints->ai_protocol == IPPROTO_SCTP)
			hints->ai_socktype = SOCK_STREAM;
#endif
	}
}
