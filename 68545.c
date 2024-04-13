cleanup_entry(struct ipt_entry *e, struct net *net)
{
	struct xt_tgdtor_param par;
	struct xt_entry_target *t;
	struct xt_entry_match *ematch;

	/* Cleanup all matches */
	xt_ematch_foreach(ematch, e)
		cleanup_match(ematch, net);
	t = ipt_get_target(e);

	par.net      = net;
	par.target   = t->u.kernel.target;
	par.targinfo = t->data;
	par.family   = NFPROTO_IPV4;
	if (par.target->destroy != NULL)
		par.target->destroy(&par);
	module_put(par.target->me);
	xt_percpu_counter_free(&e->counters);
}
