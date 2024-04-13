static int route4_dump(struct net *net, struct tcf_proto *tp, void *fh,
		       struct sk_buff *skb, struct tcmsg *t, bool rtnl_held)
{
	struct route4_filter *f = fh;
	struct nlattr *nest;
	u32 id;

	if (f == NULL)
		return skb->len;

	t->tcm_handle = f->handle;

	nest = nla_nest_start_noflag(skb, TCA_OPTIONS);
	if (nest == NULL)
		goto nla_put_failure;

	if (!(f->handle & 0x8000)) {
		id = f->id & 0xFF;
		if (nla_put_u32(skb, TCA_ROUTE4_TO, id))
			goto nla_put_failure;
	}
	if (f->handle & 0x80000000) {
		if ((f->handle >> 16) != 0xFFFF &&
		    nla_put_u32(skb, TCA_ROUTE4_IIF, f->iif))
			goto nla_put_failure;
	} else {
		id = f->id >> 16;
		if (nla_put_u32(skb, TCA_ROUTE4_FROM, id))
			goto nla_put_failure;
	}
	if (f->res.classid &&
	    nla_put_u32(skb, TCA_ROUTE4_CLASSID, f->res.classid))
		goto nla_put_failure;

	if (tcf_exts_dump(skb, &f->exts) < 0)
		goto nla_put_failure;

	nla_nest_end(skb, nest);

	if (tcf_exts_dump_stats(skb, &f->exts) < 0)
		goto nla_put_failure;

	return skb->len;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -1;
}