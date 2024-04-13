static int rtnl_net_valid_getid_req(struct sk_buff *skb,
				    const struct nlmsghdr *nlh,
				    struct nlattr **tb,
				    struct netlink_ext_ack *extack)
{
	int i, err;

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse(nlh, sizeof(struct rtgenmsg), tb, NETNSA_MAX,
				   rtnl_net_policy, extack);

	err = nlmsg_parse_strict(nlh, sizeof(struct rtgenmsg), tb, NETNSA_MAX,
				 rtnl_net_policy, extack);
	if (err)
		return err;

	for (i = 0; i <= NETNSA_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case NETNSA_PID:
		case NETNSA_FD:
		case NETNSA_NSID:
		case NETNSA_TARGET_NSID:
			break;
		default:
			NL_SET_ERR_MSG(extack, "Unsupported attribute in peer netns getid request");
			return -EINVAL;
		}
	}

	return 0;
}
