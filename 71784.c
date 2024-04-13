static int rtnl_valid_dump_net_req(const struct nlmsghdr *nlh, struct sock *sk,
				   struct rtnl_net_dump_cb *net_cb,
				   struct netlink_callback *cb)
{
	struct netlink_ext_ack *extack = cb->extack;
	struct nlattr *tb[NETNSA_MAX + 1];
	int err, i;

	err = nlmsg_parse_strict(nlh, sizeof(struct rtgenmsg), tb, NETNSA_MAX,
				 rtnl_net_policy, extack);
	if (err < 0)
		return err;

	for (i = 0; i <= NETNSA_MAX; i++) {
		if (!tb[i])
			continue;

		if (i == NETNSA_TARGET_NSID) {
			struct net *net;

			net = rtnl_get_net_ns_capable(sk, nla_get_s32(tb[i]));
			if (IS_ERR(net)) {
				NL_SET_BAD_ATTR(extack, tb[i]);
				NL_SET_ERR_MSG(extack,
					       "Invalid target network namespace id");
				return PTR_ERR(net);
			}
			net_cb->fillargs.add_ref = true;
			net_cb->ref_net = net_cb->tgt_net;
			net_cb->tgt_net = net;
		} else {
			NL_SET_BAD_ATTR(extack, tb[i]);
			NL_SET_ERR_MSG(extack,
				       "Unsupported attribute in dump request");
			return -EINVAL;
		}
	}

	return 0;
}
