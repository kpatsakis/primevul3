check_match(struct xt_entry_match *m, struct xt_mtchk_param *par)
{
	const struct ipt_ip *ip = par->entryinfo;

	par->match     = m->u.kernel.match;
	par->matchinfo = m->data;

	return xt_check_match(par, m->u.match_size - sizeof(*m),
			      ip->proto, ip->invflags & IPT_INV_PROTO);
}
