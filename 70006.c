static int ipip6_tunnel_update_6rd(struct ip_tunnel *t,
				   struct ip_tunnel_6rd *ip6rd)
{
	struct in6_addr prefix;
	__be32 relay_prefix;

	if (ip6rd->relay_prefixlen > 32 ||
	    ip6rd->prefixlen + (32 - ip6rd->relay_prefixlen) > 64)
		return -EINVAL;

	ipv6_addr_prefix(&prefix, &ip6rd->prefix, ip6rd->prefixlen);
	if (!ipv6_addr_equal(&prefix, &ip6rd->prefix))
		return -EINVAL;
	if (ip6rd->relay_prefixlen)
		relay_prefix = ip6rd->relay_prefix &
			       htonl(0xffffffffUL <<
				     (32 - ip6rd->relay_prefixlen));
	else
		relay_prefix = 0;
	if (relay_prefix != ip6rd->relay_prefix)
		return -EINVAL;

	t->ip6rd.prefix = prefix;
	t->ip6rd.relay_prefix = relay_prefix;
	t->ip6rd.prefixlen = ip6rd->prefixlen;
	t->ip6rd.relay_prefixlen = ip6rd->relay_prefixlen;
	dst_cache_reset(&t->dst_cache);
	netdev_state_change(t->dev);
	return 0;
}
