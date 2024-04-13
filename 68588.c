ip6_checkentry(const struct ip6t_ip6 *ipv6)
{
	if (ipv6->flags & ~IP6T_F_MASK)
		return false;
	if (ipv6->invflags & ~IP6T_INV_MASK)
		return false;

	return true;
}
