evutil_adjust_hints_for_addrconfig_(struct evutil_addrinfo *hints)
{
	if (!(hints->ai_flags & EVUTIL_AI_ADDRCONFIG))
		return;
	if (hints->ai_family != PF_UNSPEC)
		return;
	if (!have_checked_interfaces)
		evutil_check_interfaces(0);
	if (had_ipv4_address && !had_ipv6_address) {
		hints->ai_family = PF_INET;
	} else if (!had_ipv4_address && had_ipv6_address) {
		hints->ai_family = PF_INET6;
	}
}
