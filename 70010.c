static inline bool is_spoofed_6rd(struct ip_tunnel *tunnel, const __be32 v4addr,
				  const struct in6_addr *v6addr)
{
	__be32 v4embed = 0;
	if (check_6rd(tunnel, v6addr, &v4embed) && v4addr != v4embed)
		return true;
	return false;
}
