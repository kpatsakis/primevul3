static int ctrl_dumpfamily(struct sk_buff *skb, struct netlink_callback *cb)
{
	int n = 0;
	struct genl_family *rt;
	struct net *net = sock_net(skb->sk);
	int fams_to_skip = cb->args[0];
	unsigned int id;

	idr_for_each_entry(&genl_fam_idr, rt, id) {
		if (!rt->netnsok && !net_eq(net, &init_net))
			continue;

		if (n++ < fams_to_skip)
			continue;

		if (ctrl_fill_info(rt, NETLINK_CB(cb->skb).portid,
				   cb->nlh->nlmsg_seq, NLM_F_MULTI,
				   skb, CTRL_CMD_NEWFAMILY) < 0) {
			n--;
			break;
		}
	}

	cb->args[0] = n;
	return skb->len;
}
