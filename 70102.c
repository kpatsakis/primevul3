static int genl_family_rcv_msg(const struct genl_family *family,
			       struct sk_buff *skb,
			       struct nlmsghdr *nlh,
			       struct netlink_ext_ack *extack)
{
	const struct genl_ops *ops;
	struct net *net = sock_net(skb->sk);
	struct genl_info info;
	struct genlmsghdr *hdr = nlmsg_data(nlh);
	struct nlattr **attrbuf;
	int hdrlen, err;

	/* this family doesn't exist in this netns */
	if (!family->netnsok && !net_eq(net, &init_net))
		return -ENOENT;

	hdrlen = GENL_HDRLEN + family->hdrsize;
	if (nlh->nlmsg_len < nlmsg_msg_size(hdrlen))
		return -EINVAL;

	ops = genl_get_cmd(hdr->cmd, family);
	if (ops == NULL)
		return -EOPNOTSUPP;

	if ((ops->flags & GENL_ADMIN_PERM) &&
	    !netlink_capable(skb, CAP_NET_ADMIN))
		return -EPERM;

	if ((ops->flags & GENL_UNS_ADMIN_PERM) &&
	    !netlink_ns_capable(skb, net->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	if ((nlh->nlmsg_flags & NLM_F_DUMP) == NLM_F_DUMP) {
		int rc;

		if (ops->dumpit == NULL)
			return -EOPNOTSUPP;

		if (!family->parallel_ops) {
			struct netlink_dump_control c = {
				.module = family->module,
				/* we have const, but the netlink API doesn't */
				.data = (void *)ops,
				.start = genl_lock_start,
				.dump = genl_lock_dumpit,
				.done = genl_lock_done,
			};

			genl_unlock();
			rc = __netlink_dump_start(net->genl_sock, skb, nlh, &c);
			genl_lock();

		} else {
			struct netlink_dump_control c = {
				.module = family->module,
				.start = ops->start,
				.dump = ops->dumpit,
				.done = ops->done,
			};

			rc = __netlink_dump_start(net->genl_sock, skb, nlh, &c);
		}

		return rc;
	}

	if (ops->doit == NULL)
		return -EOPNOTSUPP;

	if (family->maxattr && family->parallel_ops) {
		attrbuf = kmalloc_array(family->maxattr + 1,
					sizeof(struct nlattr *),
					GFP_KERNEL);
		if (attrbuf == NULL)
			return -ENOMEM;
	} else
		attrbuf = family->attrbuf;

	if (attrbuf) {
		err = nlmsg_parse(nlh, hdrlen, attrbuf, family->maxattr,
				  ops->policy, extack);
		if (err < 0)
			goto out;
	}

	info.snd_seq = nlh->nlmsg_seq;
	info.snd_portid = NETLINK_CB(skb).portid;
	info.nlhdr = nlh;
	info.genlhdr = nlmsg_data(nlh);
	info.userhdr = nlmsg_data(nlh) + GENL_HDRLEN;
	info.attrs = attrbuf;
	info.extack = extack;
	genl_info_net_set(&info, net);
	memset(&info.user_ptr, 0, sizeof(info.user_ptr));

	if (family->pre_doit) {
		err = family->pre_doit(ops, skb, &info);
		if (err)
			goto out;
	}

	err = ops->doit(skb, &info);

	if (family->post_doit)
		family->post_doit(ops, skb, &info);

out:
	if (family->parallel_ops)
		kfree(attrbuf);

	return err;
}
