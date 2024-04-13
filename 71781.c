static int rtnl_net_getid(struct sk_buff *skb, struct nlmsghdr *nlh,
			  struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(skb->sk);
	struct nlattr *tb[NETNSA_MAX + 1];
	struct net_fill_args fillargs = {
		.portid = NETLINK_CB(skb).portid,
		.seq = nlh->nlmsg_seq,
		.cmd = RTM_NEWNSID,
	};
	struct net *peer, *target = net;
	struct nlattr *nla;
	struct sk_buff *msg;
	int err;

	err = rtnl_net_valid_getid_req(skb, nlh, tb, extack);
	if (err < 0)
		return err;
	if (tb[NETNSA_PID]) {
		peer = get_net_ns_by_pid(nla_get_u32(tb[NETNSA_PID]));
		nla = tb[NETNSA_PID];
	} else if (tb[NETNSA_FD]) {
		peer = get_net_ns_by_fd(nla_get_u32(tb[NETNSA_FD]));
		nla = tb[NETNSA_FD];
	} else if (tb[NETNSA_NSID]) {
		peer = get_net_ns_by_id(net, nla_get_u32(tb[NETNSA_NSID]));
		if (!peer)
			peer = ERR_PTR(-ENOENT);
		nla = tb[NETNSA_NSID];
	} else {
		NL_SET_ERR_MSG(extack, "Peer netns reference is missing");
		return -EINVAL;
	}

	if (IS_ERR(peer)) {
		NL_SET_BAD_ATTR(extack, nla);
		NL_SET_ERR_MSG(extack, "Peer netns reference is invalid");
		return PTR_ERR(peer);
	}

	if (tb[NETNSA_TARGET_NSID]) {
		int id = nla_get_s32(tb[NETNSA_TARGET_NSID]);

		target = rtnl_get_net_ns_capable(NETLINK_CB(skb).sk, id);
		if (IS_ERR(target)) {
			NL_SET_BAD_ATTR(extack, tb[NETNSA_TARGET_NSID]);
			NL_SET_ERR_MSG(extack,
				       "Target netns reference is invalid");
			err = PTR_ERR(target);
			goto out;
		}
		fillargs.add_ref = true;
		fillargs.ref_nsid = peernet2id(net, peer);
	}

	msg = nlmsg_new(rtnl_net_get_size(), GFP_KERNEL);
	if (!msg) {
		err = -ENOMEM;
		goto out;
	}

	fillargs.nsid = peernet2id(target, peer);
	err = rtnl_net_fill(msg, &fillargs);
	if (err < 0)
		goto err_out;

	err = rtnl_unicast(msg, net, NETLINK_CB(skb).portid);
	goto out;

err_out:
	nlmsg_free(msg);
out:
	if (fillargs.add_ref)
		put_net(target);
	put_net(peer);
	return err;
}
