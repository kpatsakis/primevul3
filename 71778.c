static int rtnl_net_dumpid(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct rtnl_net_dump_cb net_cb = {
		.tgt_net = sock_net(skb->sk),
		.skb = skb,
		.fillargs = {
			.portid = NETLINK_CB(cb->skb).portid,
			.seq = cb->nlh->nlmsg_seq,
			.flags = NLM_F_MULTI,
			.cmd = RTM_NEWNSID,
		},
		.idx = 0,
		.s_idx = cb->args[0],
	};
	int err = 0;

	if (cb->strict_check) {
		err = rtnl_valid_dump_net_req(cb->nlh, skb->sk, &net_cb, cb);
		if (err < 0)
			goto end;
	}

	spin_lock_bh(&net_cb.tgt_net->nsid_lock);
	if (net_cb.fillargs.add_ref &&
	    !net_eq(net_cb.ref_net, net_cb.tgt_net) &&
	    !spin_trylock_bh(&net_cb.ref_net->nsid_lock)) {
		spin_unlock_bh(&net_cb.tgt_net->nsid_lock);
		err = -EAGAIN;
		goto end;
	}
	idr_for_each(&net_cb.tgt_net->netns_ids, rtnl_net_dumpid_one, &net_cb);
	if (net_cb.fillargs.add_ref &&
	    !net_eq(net_cb.ref_net, net_cb.tgt_net))
		spin_unlock_bh(&net_cb.ref_net->nsid_lock);
	spin_unlock_bh(&net_cb.tgt_net->nsid_lock);

	cb->args[0] = net_cb.idx;
end:
	if (net_cb.fillargs.add_ref)
		put_net(net_cb.tgt_net);
	return err < 0 ? err : skb->len;
}
