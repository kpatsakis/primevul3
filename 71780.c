static int rtnl_net_fill(struct sk_buff *skb, struct net_fill_args *args)
{
	struct nlmsghdr *nlh;
	struct rtgenmsg *rth;

	nlh = nlmsg_put(skb, args->portid, args->seq, args->cmd, sizeof(*rth),
			args->flags);
	if (!nlh)
		return -EMSGSIZE;

	rth = nlmsg_data(nlh);
	rth->rtgen_family = AF_UNSPEC;

	if (nla_put_s32(skb, NETNSA_NSID, args->nsid))
		goto nla_put_failure;

	if (args->add_ref &&
	    nla_put_s32(skb, NETNSA_CURRENT_NSID, args->ref_nsid))
		goto nla_put_failure;

	nlmsg_end(skb, nlh);
	return 0;

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}
