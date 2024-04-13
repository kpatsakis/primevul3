compat_find_calc_match(struct xt_entry_match *m,
		       const struct ip6t_ip6 *ipv6,
		       int *size)
{
	struct xt_match *match;

	match = xt_request_find_match(NFPROTO_IPV6, m->u.user.name,
				      m->u.user.revision);
	if (IS_ERR(match))
		return PTR_ERR(match);

	m->u.kernel.match = match;
	*size += xt_compat_match_offset(match);
	return 0;
}
