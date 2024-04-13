static int check_match(struct xt_entry_match *m, struct xt_mtchk_param *par)
{
	const struct ip6t_ip6 *ipv6 = par->entryinfo;

	par->match     = m->u.kernel.match;
	par->matchinfo = m->data;

	return xt_check_match(par, m->u.match_size - sizeof(*m),
			      ipv6->proto, ipv6->invflags & IP6T_INV_PROTO);
}
