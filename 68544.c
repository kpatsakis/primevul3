static int check_target(struct ipt_entry *e, struct net *net, const char *name)
{
	struct xt_entry_target *t = ipt_get_target(e);
	struct xt_tgchk_param par = {
		.net       = net,
		.table     = name,
		.entryinfo = e,
		.target    = t->u.kernel.target,
		.targinfo  = t->data,
		.hook_mask = e->comefrom,
		.family    = NFPROTO_IPV4,
	};

	return xt_check_target(&par, t->u.target_size - sizeof(*t),
			       e->ip.proto, e->ip.invflags & IPT_INV_PROTO);
}
