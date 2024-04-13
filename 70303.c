daemon_seraddr(struct sockaddr_storage *sockaddrin, struct rpcap_sockaddr *sockaddrout)
{
	memset(sockaddrout, 0, sizeof(struct sockaddr_storage));

	if (sockaddrin == NULL) return;

	switch (sockaddrin->ss_family)
	{
	case AF_INET:
		{
		struct sockaddr_in *sockaddrin_ipv4;
		struct rpcap_sockaddr_in *sockaddrout_ipv4;

		sockaddrin_ipv4 = (struct sockaddr_in *) sockaddrin;
		sockaddrout_ipv4 = (struct rpcap_sockaddr_in *) sockaddrout;
		sockaddrout_ipv4->family = htons(RPCAP_AF_INET);
		sockaddrout_ipv4->port = htons(sockaddrin_ipv4->sin_port);
		memcpy(&sockaddrout_ipv4->addr, &sockaddrin_ipv4->sin_addr, sizeof(sockaddrout_ipv4->addr));
		memset(sockaddrout_ipv4->zero, 0, sizeof(sockaddrout_ipv4->zero));
		break;
		}

#ifdef AF_INET6
	case AF_INET6:
		{
		struct sockaddr_in6 *sockaddrin_ipv6;
		struct rpcap_sockaddr_in6 *sockaddrout_ipv6;

		sockaddrin_ipv6 = (struct sockaddr_in6 *) sockaddrin;
		sockaddrout_ipv6 = (struct rpcap_sockaddr_in6 *) sockaddrout;
		sockaddrout_ipv6->family = htons(RPCAP_AF_INET6);
		sockaddrout_ipv6->port = htons(sockaddrin_ipv6->sin6_port);
		sockaddrout_ipv6->flowinfo = htonl(sockaddrin_ipv6->sin6_flowinfo);
		memcpy(&sockaddrout_ipv6->addr, &sockaddrin_ipv6->sin6_addr, sizeof(sockaddrout_ipv6->addr));
		sockaddrout_ipv6->scope_id = htonl(sockaddrin_ipv6->sin6_scope_id);
		break;
		}
#endif
	}
}
