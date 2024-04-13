void genl_notify(const struct genl_family *family, struct sk_buff *skb,
		 struct genl_info *info, u32 group, gfp_t flags)
{
	struct net *net = genl_info_net(info);
	struct sock *sk = net->genl_sock;
	int report = 0;

	if (info->nlhdr)
		report = nlmsg_report(info->nlhdr);

	if (WARN_ON_ONCE(group >= family->n_mcgrps))
		return;
	group = family->mcgrp_offset + group;
	nlmsg_notify(sk, skb, info->snd_portid, group, report, flags);
}
