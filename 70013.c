static bool only_dnatted(const struct ip_tunnel *tunnel,
	const struct in6_addr *v6dst)
{
	int prefix_len;

#ifdef CONFIG_IPV6_SIT_6RD
	prefix_len = tunnel->ip6rd.prefixlen + 32
		- tunnel->ip6rd.relay_prefixlen;
#else
	prefix_len = 48;
#endif
	return ipv6_chk_custom_prefix(v6dst, prefix_len, tunnel->dev);
}
