static int genl_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh,
			struct netlink_ext_ack *extack)
{
	const struct genl_family *family;
	int err;

	family = genl_family_find_byid(nlh->nlmsg_type);
	if (family == NULL)
		return -ENOENT;

	if (!family->parallel_ops)
		genl_lock();

	err = genl_family_rcv_msg(family, skb, nlh, extack);

	if (!family->parallel_ops)
		genl_unlock();

	return err;
}
